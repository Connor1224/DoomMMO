// This code contains NVIDIA Confidential Information and is disclosed to you
// under a form of NVIDIA software license agreement provided separately to you.
//
// Notice
// NVIDIA Corporation and its licensors retain all intellectual property and
// proprietary rights in and to this software and related documentation and
// any modifications thereto. Any use, reproduction, disclosure, or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA Corporation is strictly prohibited.
//
// ALL NVIDIA DESIGN SPECIFICATIONS, CODE ARE PROVIDED "AS IS.". NVIDIA MAKES
// NO WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES OF NONINFRINGEMENT,
// MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Information and code furnished is believed to be accurate and reliable.
// However, NVIDIA Corporation assumes no responsibility for the consequences of use of such
// information or for any infringement of patents or other rights of third parties that may
// result from its use. No license is granted by implication or otherwise under any patent
// or patent rights of NVIDIA Corporation. Details are subject to change without notice.
// This code supersedes and replaces all information previously supplied.
// NVIDIA Corporation products are not authorized for use as critical
// components in life support devices or systems without express written approval of
// NVIDIA Corporation.
//
// Copyright (c) 2008-2013 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  
#include "SnXmlImpl.h"
#include "PsHash.h"
#include "PsHashMap.h"
#include "SnSimpleXmlWriter.h"
#include "PsSort.h"
#include "FastXml.h"
#include "PxFileBuffer.h"
#include "PsString.h"
#include "SnXmlMemoryPool.h"
#include "PxExtensionMetaDataObjects.h"
#include "SnXmlVisitorWriter.h"
#include "SnXmlVisitorReader.h"
#include "SnPxFilebufToPxStream.h"
#include "SnXmlMemoryAllocator.h"
#include "SnXmlStringToType.h"
#include "PsString.h"
#include "SnRepXCollection.h"
#include "SnRepXUpgrader.h"
#include "../SnSerializationRegistry.h"

using namespace physx;
using namespace FAST_XML;

using namespace physx::profile; //for the foundation wrapper systme.

namespace
{
	class FileBufFromPxInputData: public PxFileBuf
	{
	public:
		FileBufFromPxInputData(PxInputData& data): mData(data) {}

		OpenMode getOpenMode(void) const		{	return OPEN_READ_ONLY;				}
		SeekType isSeekable(void) const			{	return SEEKABLE_READ;				}
		PxU32 getFileLength(void) const			{	return mData.getLength();			}
		PxU32 seekRead(PxU32 loc)				{	mData.seek(loc); return tellRead();	}
		PxU32 seekWrite(PxU32 /*loc*/)				{	PX_ASSERT(0);	return 0;		}
		PxU32 read(void *mem,PxU32 len)			{	return mData.read(mem, len);		}
		PxU32 write(const void* /*mem*/, PxU32 /*len*/)	{	PX_ASSERT(0); return 0;		}
		PxU32 tellRead(void) const				{	return mData.tell();				}
		PxU32 tellWrite(void) const				{	PX_ASSERT(0); return 0;				}
		void flush(void)						{	PX_ASSERT(0);						}
		void close(void)						{	}

		PxU32 peek(void *mem,PxU32 len)
		{
			PxU32 here = mData.tell();
			PxU32 length = mData.read(mem, len);
			mData.seek(here);
			return length;
		}

		PxInputData& mData;
	private:
		FileBufFromPxInputData& operator=(const FileBufFromPxInputData&);
	};
}

namespace physx { namespace Sn {	
	const char* GetRepXErrorString( PxRepXErrorCode::Enum errCode )
	{
		static const char* errorString[] =
		{
#define	PX_REPX_DEFINE_ERROR_CODE(x)	#x,
#include "SnRepXErrorCodeDefs.h"
#undef	PX_REPX_DEFINE_ERROR_CODE
		};

		PxU32 index = (PxU32)errCode;
		PX_ASSERT( index < sizeof errorString / sizeof errorString[0] );

		return errorString[index];
	}

	PxRepXErrorCode::Enum ReportError( PxRepXErrorCode::Enum errCode, const char* context, const char* file, int line )
	{
		if ( PxRepXErrorCode::eSuccess != errCode )
		{
			PxErrorCallback& callback = PxGetFoundation().getErrorCallback();
			const char* message = GetRepXErrorString( errCode );
			callback.reportError(PxErrorCode::eINVALID_OPERATION, message, file, line);
			if ( NULL != context )
				callback.reportError(PxErrorCode::eDEBUG_INFO, context, file, line);
		}

		return errCode;
	}

	class XmlNodeWriter : public SimpleXmlWriter
	{
		XmlMemoryAllocatorImpl&	mParseAllocator;
		XmlNode*					mCurrentNode;
		XmlNode*					mTopNode;
		PxU32						mTabCount;

	public:
		XmlNodeWriter( XmlMemoryAllocatorImpl& inAllocator, PxU32 inTabCount = 0 ) 
			: mParseAllocator( inAllocator ) 
			, mCurrentNode( NULL )
			, mTopNode( NULL )
			, mTabCount( inTabCount )
		{}
		virtual ~XmlNodeWriter(){}
		void onNewNode( XmlNode* newNode )
		{
			if ( mCurrentNode != NULL )
				mCurrentNode->addChild( newNode );
			if ( mTopNode == NULL )
				mTopNode = newNode;
			mCurrentNode = newNode;
			++mTabCount;
		}

		XmlNode* getTopNode() const { return mTopNode; }

		virtual void beginTag( const char* inTagname )
		{
			onNewNode( allocateRepXNode( &mParseAllocator.mManager, inTagname, NULL ) );
		}
		virtual void endTag()
		{
			if ( mCurrentNode )
				mCurrentNode = mCurrentNode->mParent;
			if ( mTabCount )
				--mTabCount;
		}
		virtual void addAttribute( const char*, const char* )
		{
			PX_ASSERT( false );
		}
		virtual void writeContentTag( const char* inTag, const char* inContent )
		{
			onNewNode( allocateRepXNode( &mParseAllocator.mManager, inTag, inContent ) );
			endTag();
		}
		virtual void addContent( const char* inContent )
		{
			if ( mCurrentNode->mData )
				releaseStr( &mParseAllocator.mManager, mCurrentNode->mData );
			mCurrentNode->mData = copyStr( &mParseAllocator.mManager, inContent );
		}
		virtual PxU32 tabCount() { return mTabCount; }
	};

	struct XmlWriterImpl : public XmlWriter
	{
		PxU32				mTagDepth;
		SimpleXmlWriter*	mWriter;
		MemoryBuffer*		mMemBuffer;

		XmlWriterImpl( SimpleXmlWriter* inWriter, MemoryBuffer* inMemBuffer )
			: mTagDepth( 0 )
			, mWriter( inWriter )
			, mMemBuffer( inMemBuffer )
		{
		}
		~XmlWriterImpl()
		{
			while( mTagDepth )
			{
				--mTagDepth;
				mWriter->endTag();
			}
		}
		virtual void write( const char* inName, const char* inData )
		{
			mWriter->writeContentTag( inName, inData );
		}
		virtual void write( const char* inName, const PxRepXObject& inLiveObject )
		{
			(*mMemBuffer) << inLiveObject.mId;
			writeProperty( *mWriter, *mMemBuffer, inName );
		}
		virtual void addAndGotoChild( const char* inName )
		{
			mWriter->beginTag( inName );
			mTagDepth++;
		}
		virtual void leaveChild()
		{
			if ( mTagDepth )
			{
				mWriter->endTag();
				--mTagDepth;
			}
		}
	};

	struct XmlParseArgs
	{
		XmlMemoryAllocatorImpl*					mAllocator;
		ProfileArray<RepXCollectionItem>*		mCollection;
		
		XmlParseArgs( XmlMemoryAllocatorImpl* inAllocator
			, ProfileArray<RepXCollectionItem>* inCollection)
			: mAllocator( inAllocator )
			, mCollection( inCollection )
		{
		}
	};

	struct XmlNodeReader : public XmlReaderWriter
	{
		FoundationWrapper mWrapper;
		CMemoryPoolManager& mManager;
		XmlNode* mCurrentNode;
		XmlNode* mTopNode;
		ProfileArray<XmlNode*> mContext;
		XmlNodeReader( XmlNode* inCurrentNode, PxAllocatorCallback& inAllocator, CMemoryPoolManager& nodePoolManager )
			: mWrapper( inAllocator )
			, mManager( nodePoolManager )
			, mCurrentNode( inCurrentNode )
			, mTopNode( inCurrentNode )
			, mContext( mWrapper )
		{
		}

		//Does this node exist as data in the format.
		virtual bool read( const char* inName, const char*& outData )
		{
			XmlNode* theChild( mCurrentNode->findChildByName( inName ) );
			if ( theChild )
			{
				outData = theChild->mData;
				return outData && *outData;
			}
			return false;
		}

		virtual bool read( const char* inName, PxRepXId& outId )
		{
			XmlNode* theChild( mCurrentNode->findChildByName( inName ) );
			if ( theChild )
			{
				const char* theValue( theChild->mData );
				strto( outId, theValue );
				return true;
			}
			return false;
		}

		virtual bool gotoChild( const char* inName )
		{
			XmlNode* theChild( mCurrentNode->findChildByName( inName ) );
			if ( theChild )
			{
				mCurrentNode =theChild;
				return true;
			}
			return false;
		}
		virtual bool gotoFirstChild()
		{
			if ( mCurrentNode->mFirstChild )
			{
				mCurrentNode = mCurrentNode->mFirstChild;
				return true;
			}
			return false;
		}
		virtual bool gotoNextSibling()
		{
			if ( mCurrentNode->mNextSibling )
			{
				mCurrentNode = mCurrentNode->mNextSibling;
				return true;
			}
			return false;
		}
		virtual PxU32 countChildren()
		{
			PxU32 retval=  0;
			for ( XmlNode* theChild = mCurrentNode->mFirstChild; theChild != NULL; theChild = theChild->mNextSibling )
				++retval;
			return retval;
		}
		virtual const char* getCurrentItemName()
		{
			return mCurrentNode->mName;
		}
		virtual const char* getCurrentItemValue()
		{
			return mCurrentNode->mData;
		}

		virtual bool leaveChild()
		{
			if ( mCurrentNode != mTopNode && mCurrentNode->mParent )
			{
				mCurrentNode = mCurrentNode->mParent;
				return true;
			}
			return false;
		}

		virtual void pushCurrentContext()
		{
			mContext.pushBack( mCurrentNode );
		}
		virtual void popCurrentContext()
		{
			if ( mContext.size() )
			{
				mCurrentNode = mContext.back();
				mContext.popBack();
			}
		}

		virtual void setNode( XmlNode& inNode )
		{
			mContext.clear();
			mCurrentNode = &inNode;
			mTopNode = mCurrentNode;
		}

		virtual void addOrGotoChild( const char* inName )
		{
			if ( gotoChild( inName )== false )
			{
				XmlNode* newNode = allocateRepXNode( &mManager, inName, NULL );
				mCurrentNode->addChild( newNode );
				mCurrentNode = newNode;
			}
		}
		virtual void setCurrentItemValue( const char* inValue )
		{
			mCurrentNode->mData = copyStr( &mManager, inValue ); 
		}
		virtual bool removeChild( const char* name )
		{
			XmlNode* theChild( mCurrentNode->findChildByName( name ) );
			if ( theChild )
			{
				releaseNodeAndChildren( &mManager, theChild );
				return true;
			}
			return false;
		}
		virtual void release() { this->~XmlNodeReader(); mWrapper.getAllocator().deallocate(this); }

	private:
		XmlNodeReader& operator=(const XmlNodeReader&);
	};

	PX_INLINE void  freeNodeAndChildren( XmlNode* tempNode, TMemoryPoolManager& inManager )
	{
		for( XmlNode* theNode = tempNode->mFirstChild; theNode != NULL; theNode = theNode->mNextSibling )
			freeNodeAndChildren( theNode, inManager );
		tempNode->orphan();
		release( &inManager, tempNode );
	}

	class XmlParser : public FastXml::Callback
	{
		XmlParseArgs			mParseArgs;
		//For parse time only allocations
		XmlMemoryAllocatorImpl& mParseAllocator;
		XmlNode* mCurrentNode;
		XmlNode* mTopNode;

	public:
		XmlParser( XmlParseArgs inArgs, XmlMemoryAllocatorImpl& inParseAllocator )
			: mParseArgs( inArgs )
			, mParseAllocator( inParseAllocator )
			, mCurrentNode( NULL )
			, mTopNode( NULL )
		{
		}

		virtual ~XmlParser(){}

		virtual bool processComment(const char* /*comment*/) { return true; }
		// 'element' is the name of the element that is being closed.
		// depth is the recursion depth of this element.
		// Return true to continue processing the XML file.
		// Return false to stop processing the XML file; leaves the read pointer of the stream right after this close tag.
		// The bool 'isError' indicates whether processing was stopped due to an error, or intentionally canceled early.
		virtual bool processClose(const char* /*element*/,physx::PxU32 /*depth*/,bool& /*isError*/)
		{
			mCurrentNode = mCurrentNode->mParent;
			return true;
		}

		// return true to continue processing the XML document, false to skip.
		virtual bool processElement(
			const char *elementName,   // name of the element
			physx::PxI32 argc,         // number of attributes pairs
			const char **argv,         // list of attributes.
			const char  *elementData,  // element data, null if none
			physx::PxI32 /*lineno*/)
		{
			XmlNode* newNode = allocateRepXNode( &mParseAllocator.mManager, elementName, elementData );
			if ( mCurrentNode )
				mCurrentNode->addChild( newNode );
			mCurrentNode = newNode;
			//Add the elements as children.
			for( PxI32 item = 0; item < argc; item += 2, argv += 2 )
			{
				XmlNode* newNode = allocateRepXNode( &mParseAllocator.mManager, argv[0], argv[1] );
				mCurrentNode->addChild( newNode );
			}
			if ( mTopNode == NULL ) mTopNode = newNode;
			return true;
		}

		XmlNode* getTopNode() { return mTopNode; }

		virtual void *  fastxml_malloc(physx::PxU32 size) { if ( size ) return mParseAllocator.allocate(size); return NULL; }
		virtual void	fastxml_free(void *mem) { if ( mem ) mParseAllocator.deallocate(reinterpret_cast<PxU8*>(mem)); }

	private:
		XmlParser& operator=(const XmlParser&);
	};

	struct RepXCollectionSharedData
	{
		FoundationWrapper				mWrapper;
		XmlMemoryAllocatorImpl			mAllocator;
		PxU32							mRefCount;

		RepXCollectionSharedData( PxAllocatorCallback& inAllocator )
			: mWrapper( inAllocator )
			, mAllocator( inAllocator )
			, mRefCount( 0 )
		{
		}
		~RepXCollectionSharedData() {}
		
		void addRef() { ++mRefCount;}
		void release()
		{
			if ( mRefCount ) --mRefCount;
			if ( !mRefCount ) { this->~RepXCollectionSharedData(); mWrapper.getAllocator().deallocate(this);} 
		}
	};

	struct SharedDataPtr
	{
		RepXCollectionSharedData* mData;
		SharedDataPtr( RepXCollectionSharedData* inData )
			: mData( inData )
		{
			mData->addRef();
		}
		SharedDataPtr( const SharedDataPtr& inOther )
			: mData( inOther.mData )
		{
			mData->addRef();
		}
		SharedDataPtr& operator=( const SharedDataPtr& inOther );
		~SharedDataPtr()
		{
			mData->release();
			mData = NULL;
		}
		RepXCollectionSharedData* operator->() { return mData; }
		const RepXCollectionSharedData* operator->() const { return mData; }
	};
	
	class RepXCollectionImpl : public RepXCollection, public UserAllocated
	{
		SharedDataPtr							mSharedData;

		XmlMemoryAllocatorImpl&					mAllocator;
		PxSerializationRegistry&				mSerializationRegistry;
		ProfileArray<RepXCollectionItem>		mCollection;
		TMemoryPoolManager						mSerializationManager;
		MemoryBuffer							mPropertyBuffer;
		PxTolerancesScale						mScale;
		PxVec3									mUpVector;
		const char*								mVersionStr;
		PxCollection*							mPxCollection;
		
	public:
		RepXCollectionImpl( PxSerializationRegistry& inRegistry, PxAllocatorCallback& inAllocator, PxCollection& inPxCollection )
			: mSharedData( PX_NEW_REPX_SERIALIZER( RepXCollectionSharedData ))
			, mAllocator( mSharedData->mAllocator )
			, mSerializationRegistry( inRegistry )
			, mCollection( mSharedData->mWrapper )
			, mSerializationManager( inAllocator )
			, mPropertyBuffer( &mSerializationManager )
			, mUpVector( 0,0,0 )
			, mVersionStr( getLatestVersion() )
			, mPxCollection( &inPxCollection )
		{
			memset( &mScale, 0, sizeof( PxTolerancesScale ) );
			PX_ASSERT( mScale.isValid() == false );
		}

		RepXCollectionImpl( PxSerializationRegistry& inRegistry, const RepXCollectionImpl& inSrc, const char* inNewVersion )
			: mSharedData( inSrc.mSharedData )
			, mAllocator( mSharedData->mAllocator )
			, mSerializationRegistry( inRegistry )
			, mCollection( mSharedData->mWrapper )
			, mSerializationManager( mSharedData->mWrapper.getAllocator() )
			, mPropertyBuffer( &mSerializationManager )
			, mScale( inSrc.mScale )
			, mUpVector( inSrc.mUpVector )
			, mVersionStr( inNewVersion )
			, mPxCollection( NULL )
		{
		}

		virtual ~RepXCollectionImpl()
		{
			PxU32 numItems = mCollection.size();
			for ( PxU32 idx = 0; idx < numItems; ++idx )
			{
				XmlNode* theNode = mCollection[idx].mDescriptor;
				releaseNodeAndChildren( &mAllocator.mManager, theNode );
			}
		}

		virtual void destroy() 
		{ 
			FoundationWrapper tempWrapper( mSharedData->mWrapper.getAllocator() );
			this->~RepXCollectionImpl();
			tempWrapper.getAllocator().deallocate(this); 
		}
		
		virtual void setTolerancesScale(const PxTolerancesScale& inScale) {  mScale = inScale; }
		virtual PxTolerancesScale getTolerancesScale() const { return mScale; }
		virtual void setUpVector( const PxVec3& inUpVector ) { mUpVector = inUpVector; }
		virtual PxVec3 getUpVector() const { return mUpVector; }

	
		PX_INLINE RepXCollectionItem findItemBySceneItem( const PxRepXObject& inObject ) const
		{
			//See if the object is in the collection
			for ( PxU32 idx =0; idx < mCollection.size(); ++idx )
				if ( mCollection[idx].mLiveObject.mLiveObject == inObject.mLiveObject )
					return mCollection[idx];
			return RepXCollectionItem();
		}

		virtual RepXAddToCollectionResult addRepXObjectToCollection( const PxRepXObject& inObject, PxCollection* inCollection, PxRepXInstantiationArgs& inArgs )
		{
			PX_ASSERT( inObject.mLiveObject );
			PX_ASSERT( inObject.mId );
			if ( inObject.mLiveObject == NULL || inObject.mId == 0 )
				return RepXAddToCollectionResult( RepXAddToCollectionResult::InvalidParameters );
		
			PxRepXSerializer* theSerializer = mSerializationRegistry.getRepXSerializer( inObject.mTypeName );
			if ( theSerializer == NULL )
				return RepXAddToCollectionResult( RepXAddToCollectionResult::SerializerNotFound );

			RepXCollectionItem existing = findItemBySceneItem( inObject );
			if ( existing.mLiveObject.mLiveObject )
				return RepXAddToCollectionResult( RepXAddToCollectionResult::AlreadyInCollection, existing.mLiveObject.mId );
			
			XmlNodeWriter theXmlWriter( mAllocator, 1 );
			XmlWriterImpl theRepXWriter( &theXmlWriter, &mPropertyBuffer );
			{
				SimpleXmlWriter::STagWatcher theWatcher( theXmlWriter, inObject.mTypeName );
				writeProperty( theXmlWriter, mPropertyBuffer, "Id", inObject.mId  );
				theSerializer->objectToFile( inObject, inCollection, theRepXWriter, mPropertyBuffer,inArgs );
			}
			mCollection.pushBack( RepXCollectionItem( inObject, theXmlWriter.getTopNode() ) );
			return RepXAddToCollectionResult( RepXAddToCollectionResult::Success, inObject.mId );
		}

		virtual PxRepXErrorCode::Enum instantiateCollection( PxRepXInstantiationArgs& inArgs, PxCollection* inCollection )
		{
			for ( PxU32 idx =0; idx < mCollection.size(); ++idx )
			{
				RepXCollectionItem theItem( mCollection[idx] );
				PxRepXSerializer* theSerializer = mSerializationRegistry.getRepXSerializer( theItem.mLiveObject.mTypeName );
				if ( !theSerializer )
					REPX_REPORT_ERROR_RET( PxRepXErrorCode::eSerializerNotFound, theItem.mLiveObject.mTypeName );
				else
				{
					XmlNodeReader theReader( theItem.mDescriptor, mAllocator.getAllocator(), mAllocator.mManager );
					XmlMemoryAllocatorImpl instantiationAllocator( mAllocator.getAllocator() );
					PxRepXObject theLiveObject = theSerializer->fileToObject( theReader, instantiationAllocator, inArgs, inCollection );
					if ( !theLiveObject.isValid() )
						REPX_REPORT_ERROR_RET( PxRepXErrorCode::eInvalidParameters, theLiveObject.mTypeName );
					else
					{
						if( inCollection )
						{
							const PxBase* s =  reinterpret_cast<const PxBase*>( theLiveObject.mLiveObject ) ;
                            inCollection->add( *const_cast<PxBase*>(s), PxSerialObjectRef( theItem.mLiveObject.mId ));
                        }
					}
				}
			}

			return PxRepXErrorCode::eSuccess;
		}

		void saveXmlNode( XmlNode* inNode, SimpleXmlWriter& inWriter )
		{
			XmlNode* theNode( inNode );
			if ( theNode->mData && *theNode->mData && theNode->mFirstChild == NULL )
				inWriter.writeContentTag( theNode->mName, theNode->mData );
			else
			{
				inWriter.beginTag( theNode->mName );
				if ( theNode->mData && *theNode->mData )
					inWriter.addContent( theNode->mData );
				for ( XmlNode* theChild = theNode->mFirstChild; 
						theChild != NULL;
						theChild = theChild->mNextSibling )
					saveXmlNode( theChild, inWriter );
				inWriter.endTag();
			}
		}

		virtual void save( PxOutputStream& inStream )
		{
			SimpleXmlWriterImpl<PxOutputStream> theWriter( inStream, mAllocator.getAllocator() );
			theWriter.beginTag( "PhysX30Collection" );
			theWriter.addAttribute( "version", mVersionStr );
			{
				XmlWriterImpl theRepXWriter( &theWriter, &mPropertyBuffer );
				writeProperty( theWriter, mPropertyBuffer, "UpVector", mUpVector );
				theRepXWriter.addAndGotoChild( "Scale" );
				writeAllProperties( &mScale, theRepXWriter, mPropertyBuffer, *mPxCollection);
				theRepXWriter.leaveChild();
			}
			for ( PxU32 idx =0; idx < mCollection.size(); ++idx )
			{
				RepXCollectionItem theItem( mCollection[idx] );
				XmlNode* theNode( theItem.mDescriptor );
				saveXmlNode( theNode, theWriter );
			}
		}

		void load( PxFileBuf& inFileBuf, PxSerializationRegistry& s )
		{
			inFileBuf.seekRead(0);
			XmlParser theParser( XmlParseArgs( &mAllocator, &mCollection ), mAllocator );
			FastXml* theFastXml = createFastXml( &theParser );
			theFastXml->processXml( inFileBuf );
			XmlNode* theTopNode = theParser.getTopNode();
			if ( theTopNode != NULL )
			{
				{
					
					XmlMemoryAllocatorImpl instantiationAllocator( mAllocator.getAllocator() );
					XmlNodeReader theReader( theTopNode, mAllocator.getAllocator(), mAllocator.mManager );
					readProperty( theReader, "UpVector", mUpVector );
					if ( theReader.gotoChild( "Scale" ) )
					{
						readAllProperties( PxRepXInstantiationArgs( s.getPhysics() ), theReader, &mScale, instantiationAllocator, *mPxCollection);
						theReader.leaveChild();
					}
					const char* verStr = NULL;
					if ( theReader.read( "version", verStr ) )
						mVersionStr = verStr;
				}
				for ( XmlNode* theChild = theTopNode->mFirstChild; 
						theChild != NULL;
						theChild = theChild->mNextSibling )
				{
					if ( physx::PxStricmp( theChild->mName, "scale" ) == 0 
						|| physx::PxStricmp( theChild->mName, "version" ) == 0 
						|| physx::PxStricmp( theChild->mName, "upvector" ) == 0 )
						continue;
					XmlNodeReader theReader( theChild, mAllocator.getAllocator(), mAllocator.mManager );
					PxRepXObject theObject;
					theObject.mTypeName = theChild->mName;
					theObject.mLiveObject = NULL;
					PxRepXId theId = 0;
					theReader.read( "Id", theId );
					theObject.mId = theId;
					mCollection.pushBack( RepXCollectionItem( theObject, theChild ) );
				}
			}
		}
		
		virtual const char* getVersion() { return mVersionStr; }
		
		virtual const RepXCollectionItem* begin() const
		{
			return mCollection.begin();
		}
		virtual const RepXCollectionItem* end() const
		{
			return mCollection.end();
		}
		
		virtual RepXCollection& createCollection( const char* inVersionStr )
		{
			PxAllocatorCallback& allocator = mSharedData->mWrapper.getAllocator(); 
			RepXCollectionImpl* retval = PX_PLACEMENT_NEW((allocator.allocate(sizeof(RepXCollectionImpl), "createCollection",  __FILE__, __LINE__ )), RepXCollectionImpl) ( mSerializationRegistry, *this, inVersionStr );
		
			return *retval;
		}
		
		//Performs a deep copy of the repx node.
		virtual XmlNode* copyRepXNode( const XmlNode* srcNode ) 
		{
			return physx::Sn::copyRepXNode( &mAllocator.mManager, srcNode );
		}

		virtual void addCollectionItem( RepXCollectionItem inItem ) 
		{
			mCollection.pushBack( inItem );
		}
		
		virtual PxAllocatorCallback& getAllocator() { return mSharedData->mAllocator.getAllocator(); }
		//Create a new repx node with this name.  Its value is unset.
		virtual XmlNode& createRepXNode( const char* name )
		{
			XmlNode* newNode = allocateRepXNode( &mSharedData->mAllocator.mManager, name, NULL );
			return *newNode;
		}

		//Release this when finished.
		virtual XmlReaderWriter& createNodeEditor()
		{
			PxAllocatorCallback& allocator = mSharedData->mWrapper.getAllocator(); 
			XmlReaderWriter* retval = PX_PLACEMENT_NEW((allocator.allocate(sizeof(XmlNodeReader), "createNodeEditor",  __FILE__, __LINE__ )), XmlNodeReader) ( NULL, allocator, mAllocator.mManager );
			return *retval;
		}
	};
	
	const char* RepXCollection::getLatestVersion() { return "3.3.0"; }
	static RepXCollection* create(PxSerializationRegistry& s, PxAllocatorCallback& inAllocator, PxCollection& inCollection )
	{
		return PX_PLACEMENT_NEW((inAllocator.allocate(sizeof(RepXCollectionImpl), "RepXCollection::create",  __FILE__, __LINE__ )), RepXCollectionImpl) ( s, inAllocator, inCollection );
	}

	static RepXCollection* create(PxSerializationRegistry& s, PxInputData &data, PxAllocatorCallback& inAllocator, PxCollection& inCollection )
	{
		FileBufFromPxInputData theFileBuf(data); 	
		RepXCollectionImpl* theCollection = static_cast<RepXCollectionImpl*>( create(s, inAllocator, inCollection ) );
		theCollection->load( theFileBuf, s );
		return theCollection;
	}
}

	bool PxSerialization::serializeCollectionToXml( PxOutputStream& outputStream, PxCollection& collection, PxSerializationRegistry& sr, PxCooking* cooking, const PxCollection* externalRefs, PxXmlParserOptions* inArgs )
	{
		if( !PxSerialization::isSerializable(collection, sr, const_cast<PxCollection*>(externalRefs)) )
			return false;
		
		bool bRet = true;

		PxRepXInstantiationArgs args( sr.getPhysics(), cooking );

		PxCollection* tmpCollection = sr.getPhysics().createCollection();
		tmpCollection->add( collection );
		if(externalRefs)
		{
			tmpCollection->add(*const_cast<PxCollection*>(externalRefs));
		}
		
		PxAllocatorCallback& allocator = PxGetFoundation().getAllocatorCallback(); 
		Sn::RepXCollection* theRepXCollection = Sn::create(sr, allocator, *tmpCollection );
				
		if(inArgs != NULL)
		{
			theRepXCollection->setTolerancesScale(inArgs->mScale);
			theRepXCollection->setUpVector(inArgs->mUpVector);
		}		

		PxU32 nbObjects = collection.getNbObjects();
		if( nbObjects )
		{
		    sortCollection( collection, sr);

            for( PxU32 i = 0; i < nbObjects; i++ )
			{
                PxBase& s = collection.getObject(i);
				if( PxConcreteType::eSHAPE == s.getConcreteType() )
				{
					PxShape& shape = reinterpret_cast<PxShape&>(s);
					if( shape.isExclusive() )
						continue;
				}
				
				PxSerialObjectRef id = collection.getReference(s);
				if(id == PX_SERIAL_OBJECT_REF_INVALID)
					id = static_cast<const PxSerialObjectRef>( reinterpret_cast<const size_t>( &s ));
				
				PxRepXObject ro = createRepXObject( &s, id );
				if ( ro.mLiveObject == NULL || ro.mId == 0 )
				{
					bRet = false;
					break;
				}
					
				theRepXCollection->addRepXObjectToCollection( ro, tmpCollection, args );				
			}
		}
		tmpCollection->release();

		theRepXCollection->save(outputStream);
		theRepXCollection->destroy();
		
		
		return bRet;
	}
	
	PxCollection* PxSerialization::createCollectionFromXml(PxInputData& inputData, PxCooking& cooking, PxSerializationRegistry& sr, const PxCollection* externalRefs, PxStringTable* stringTable, PxXmlParserOptions* outArgs)
	{
		PxCollection* collection = sr.getPhysics().createCollection();
		
		if( externalRefs )
			collection->add(*const_cast<PxCollection*>(externalRefs));

		PxAllocatorCallback& allocator = PxGetFoundation().getAllocatorCallback(); 
		Sn::RepXCollection* theRepXCollection = Sn::create(sr, inputData, allocator, *collection);
		theRepXCollection = &Sn::RepXUpgrader::upgradeCollection( *theRepXCollection );
				
		PxRepXInstantiationArgs args( sr.getPhysics(), &cooking, stringTable );  
		if( Sn::PxRepXErrorCode::eSuccess != theRepXCollection->instantiateCollection(args, collection) )
		{
			collection->release();
			return NULL;
		}
		
		if( externalRefs )
			collection->remove(*const_cast<PxCollection*>(externalRefs));
		
		if(outArgs != NULL)
		{
			outArgs->mUpVector = theRepXCollection->getUpVector();
			outArgs->mScale = theRepXCollection->getTolerancesScale();
		}

		theRepXCollection->destroy();
		
		return collection;
	}
} 
