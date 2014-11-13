-- ----------------------------
-- Table structure for [dbo].[Accounts]
-- ----------------------------
CREATE TABLE [dbo].[Accounts] (
[CustomerID] int NOT NULL IDENTITY(1000000,1) ,
[email] varchar(128) NOT NULL ,
[MD5Password] varchar(32) NULL ,
[dateregistered] datetime NULL ,
[ReferralID] int NOT NULL ,
[AccountStatus] int NOT NULL DEFAULT ((100)) ,
[IsDeveloper] int NOT NULL DEFAULT ((0)) ,
[lastlogindate] datetime NULL ,
[lastloginIP] varchar(16) NULL 
)


GO
DBCC CHECKIDENT(N'[dbo].[Accounts]', RESEED, 1000015)
GO

-- ----------------------------
-- Records of Accounts
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[Accounts] ON
GO
SET IDENTITY_INSERT [dbo].[Accounts] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[Achievements]
-- ----------------------------
CREATE TABLE [dbo].[Achievements] (
[CustomerID] int NOT NULL ,
[AchID] int NOT NULL ,
[Value] int NOT NULL DEFAULT ((0)) ,
[Unlocked] int NOT NULL DEFAULT ((0)) 
)


GO

-- ----------------------------
-- Records of Achievements
-- ----------------------------
BEGIN TRANSACTION
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[Application_Revive]
-- ----------------------------
CREATE TABLE [dbo].[Application_Revive] (
[CustomerID] int NOT NULL ,
[Point] int NOT NULL ,
[PointTotal] int NOT NULL 
)


GO

-- ----------------------------
-- Records of Application_Revive
-- ----------------------------
BEGIN TRANSACTION
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[CharsCreateLog]
-- ----------------------------
CREATE TABLE [dbo].[CharsCreateLog] (
[id] int NOT NULL IDENTITY(1,1) ,
[CustomerID] int NOT NULL ,
[CharID] int NOT NULL ,
[GamerTag] nvarchar(64) NOT NULL 
)


GO

-- ----------------------------
-- Records of CharsCreateLog
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[CharsCreateLog] ON
GO
SET IDENTITY_INSERT [dbo].[CharsCreateLog] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[CharsLog]
-- ----------------------------
CREATE TABLE [dbo].[CharsLog] (
[id] int NOT NULL IDENTITY(1,1) ,
[CharMessage] nvarchar(50) NOT NULL ,
[CustomerID] int NOT NULL ,
[CharID] int NOT NULL ,
[GamerTag] nvarchar(64) NOT NULL ,
[time] datetime NOT NULL 
)


GO
DBCC CHECKIDENT(N'[dbo].[CharsLog]', RESEED, 35)
GO

-- ----------------------------
-- Records of CharsLog
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[CharsLog] ON
GO
SET IDENTITY_INSERT [dbo].[CharsLog] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[CharsStats]
-- ----------------------------
CREATE TABLE [dbo].[CharsStats] (
[CharID] int NOT NULL ,
[PlayersKilled] int NOT NULL DEFAULT ((0)) ,
[ZombiesKilled] int NOT NULL DEFAULT ((0)) 
)


GO

-- ----------------------------
-- Records of CharsStats
-- ----------------------------
BEGIN TRANSACTION
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[CheatLog]
-- ----------------------------
CREATE TABLE [dbo].[CheatLog] (
[ID] int NOT NULL IDENTITY(1,1) ,
[SessionID] bigint NOT NULL ,
[CustomerID] int NOT NULL ,
[CheatID] int NOT NULL ,
[ReportTime] datetime NOT NULL 
)


GO

-- ----------------------------
-- Records of CheatLog
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[CheatLog] ON
GO
SET IDENTITY_INSERT [dbo].[CheatLog] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[ClanApplications]
-- ----------------------------
CREATE TABLE [dbo].[ClanApplications] (
[ClanApplicationID] int NOT NULL IDENTITY(1,1) ,
[ClanID] int NOT NULL ,
[CharID] int NOT NULL ,
[ExpireTime] datetime NOT NULL ,
[ApplicationText] nvarchar(500) NOT NULL ,
[IsProcessed] int NOT NULL 
)


GO

-- ----------------------------
-- Records of ClanApplications
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[ClanApplications] ON
GO
SET IDENTITY_INSERT [dbo].[ClanApplications] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[ClanData]
-- ----------------------------
CREATE TABLE [dbo].[ClanData] (
[ClanID] int NOT NULL IDENTITY(1472,1) ,
[ClanName] nvarchar(64) NOT NULL ,
[ClanNameColor] int NOT NULL ,
[ClanTag] nvarchar(4) NOT NULL ,
[ClanTagColor] int NOT NULL ,
[ClanEmblemID] int NOT NULL ,
[ClanEmblemColor] int NOT NULL ,
[ClanXP] int NOT NULL ,
[ClanLevel] int NOT NULL ,
[ClanGP] int NOT NULL ,
[OwnerCustomerID] int NOT NULL ,
[OwnerCharID] int NOT NULL ,
[MaxClanMembers] int NOT NULL ,
[NumClanMembers] int NOT NULL ,
[ClanLore] nvarchar(512) NULL ,
[Announcement] nvarchar(512) NULL ,
[ClanCreateDate] datetime NULL 
)


GO

-- ----------------------------
-- Records of ClanData
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[ClanData] ON
GO
SET IDENTITY_INSERT [dbo].[ClanData] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[ClanEvents]
-- ----------------------------
CREATE TABLE [dbo].[ClanEvents] (
[ClanEventID] int NOT NULL IDENTITY(1,1) ,
[ClanID] int NOT NULL ,
[EventDate] datetime NOT NULL ,
[EventType] int NOT NULL ,
[EventRank] int NOT NULL ,
[Var1] int NOT NULL DEFAULT ((0)) ,
[Var2] int NOT NULL DEFAULT ((0)) ,
[Var3] int NOT NULL DEFAULT ((0)) ,
[Var4] int NOT NULL DEFAULT ((0)) ,
[Text1] nvarchar(64) NULL ,
[Text2] nvarchar(64) NULL ,
[Text3] nvarchar(256) NULL 
)


GO

-- ----------------------------
-- Records of ClanEvents
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[ClanEvents] ON
GO
SET IDENTITY_INSERT [dbo].[ClanEvents] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[ClanInvites]
-- ----------------------------
CREATE TABLE [dbo].[ClanInvites] (
[ClanInviteID] int NOT NULL IDENTITY(1,1) ,
[ClanID] int NOT NULL ,
[InviterCharID] int NOT NULL ,
[CharID] int NOT NULL ,
[ExpireTime] datetime NOT NULL 
)


GO

-- ----------------------------
-- Records of ClanInvites
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[ClanInvites] ON
GO
SET IDENTITY_INSERT [dbo].[ClanInvites] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[CRCBanList]
-- ----------------------------
CREATE TABLE [dbo].[CRCBanList] (
[CRC] varchar(50) NOT NULL 
)


GO

-- ----------------------------
-- Records of CRCBanList
-- ----------------------------
BEGIN TRANSACTION
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[DataGameRewards]
-- ----------------------------
CREATE TABLE [dbo].[DataGameRewards] (
[ID] int NOT NULL ,
[Name] nvarchar(128) NOT NULL DEFAULT '' ,
[GD_SOFT] int NOT NULL DEFAULT ((0)) ,
[XP_SOFT] int NOT NULL DEFAULT ((0)) ,
[GD_HARD] int NOT NULL DEFAULT ((0)) ,
[XP_HARD] int NOT NULL DEFAULT ((0)) 
)


GO

-- ----------------------------
-- Records of DataGameRewards
-- ----------------------------
BEGIN TRANSACTION
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[DataSkill2Price]
-- ----------------------------
CREATE TABLE [dbo].[DataSkill2Price] (
[SkillID] int NOT NULL ,
[SkillName] varchar(64) NULL DEFAULT ('SKILL DESC') ,
[Lv1] int NULL DEFAULT ((0)) ,
[Lv2] int NULL DEFAULT ((0)) ,
[Lv3] int NULL DEFAULT ((0)) ,
[Lv4] int NULL DEFAULT ((0)) ,
[Lv5] int NULL DEFAULT ((0)) 
)


GO

-- ----------------------------
-- Records of DataSkill2Price
-- ----------------------------
BEGIN TRANSACTION
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[DBG_BanLog]
-- ----------------------------
CREATE TABLE [dbo].[DBG_BanLog] (
[RecordID] int NOT NULL IDENTITY(1,1) ,
[CustomerID] int NOT NULL ,
[BanTime] datetime NULL ,
[BanDuration] int NULL ,
[BanReason] nvarchar(512) NULL 
)


GO

-- ----------------------------
-- Records of DBG_BanLog
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[DBG_BanLog] ON
GO
SET IDENTITY_INSERT [dbo].[DBG_BanLog] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[DBG_GPTransactions]
-- ----------------------------
CREATE TABLE [dbo].[DBG_GPTransactions] (
[TransactionID] int NOT NULL IDENTITY(1,1) ,
[CustomerID] int NULL ,
[TransactionTime] datetime NULL ,
[Amount] int NULL ,
[Reason] varchar(64) NULL ,
[Previous] int NULL 
)


GO
DBCC CHECKIDENT(N'[dbo].[DBG_GPTransactions]', RESEED, 87)
GO

-- ----------------------------
-- Records of DBG_GPTransactions
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[DBG_GPTransactions] ON
GO
SET IDENTITY_INSERT [dbo].[DBG_GPTransactions] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[DBG_IISApiStats]
-- ----------------------------
CREATE TABLE [dbo].[DBG_IISApiStats] (
[API] varchar(128) NOT NULL ,
[Hits] bigint NOT NULL ,
[BytesIn] bigint NOT NULL ,
[BytesOut] bigint NOT NULL 
)


GO

-- ----------------------------
-- Records of DBG_IISApiStats
-- ----------------------------
BEGIN TRANSACTION
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[DBG_LevelUpEvents]
-- ----------------------------
CREATE TABLE [dbo].[DBG_LevelUpEvents] (
[CustomerID] int NULL ,
[LevelGained] int NULL ,
[ReportTime] datetime NULL ,
[SessionID] bigint NOT NULL 
)


GO

-- ----------------------------
-- Records of DBG_LevelUpEvents
-- ----------------------------
BEGIN TRANSACTION
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[DBG_LootRewards]
-- ----------------------------
CREATE TABLE [dbo].[DBG_LootRewards] (
[RecordID] int NOT NULL IDENTITY(1,1) ,
[ReportTime] datetime NOT NULL ,
[CustomerID] int NOT NULL ,
[Roll] float(53) NOT NULL ,
[LootID] float(53) NOT NULL ,
[ItemID] int NOT NULL ,
[ExpDays] int NOT NULL ,
[GD] int NOT NULL 
)


GO

-- ----------------------------
-- Records of DBG_LootRewards
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[DBG_LootRewards] ON
GO
SET IDENTITY_INSERT [dbo].[DBG_LootRewards] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[DBG_PasswordResets]
-- ----------------------------
CREATE TABLE [dbo].[DBG_PasswordResets] (
[ResetID] int NOT NULL IDENTITY(1,1) ,
[CustomerID] int NULL ,
[ResetDate] datetime NULL ,
[NewPassword] varchar(200) NULL 
)


GO

-- ----------------------------
-- Records of DBG_PasswordResets
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[DBG_PasswordResets] ON
GO
SET IDENTITY_INSERT [dbo].[DBG_PasswordResets] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[DBG_SrvLogInfo]
-- ----------------------------
CREATE TABLE [dbo].[DBG_SrvLogInfo] (
[RecordID] int NOT NULL IDENTITY(1,1) ,
[ReportTime] datetime NULL ,
[IsProcessed] int NULL ,
[CustomerID] int NULL ,
[CharID] int NULL DEFAULT ((0)) ,
[CustomerIP] varchar(64) NULL ,
[GameSessionID] bigint NULL ,
[CheatID] int NULL ,
[RepeatCount] int NULL ,
[Gamertag] nvarchar(64) NULL DEFAULT '' ,
[Msg] varchar(512) NULL ,
[Data] varchar(4096) NULL 
)


GO
DBCC CHECKIDENT(N'[dbo].[DBG_SrvLogInfo]', RESEED, 295)
GO

-- ----------------------------
-- Records of DBG_SrvLogInfo
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[DBG_SrvLogInfo] ON
GO
SET IDENTITY_INSERT [dbo].[DBG_SrvLogInfo] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[DBG_UserRoundResults]
-- ----------------------------
CREATE TABLE [dbo].[DBG_UserRoundResults] (
[IP] varchar(32) NOT NULL ,
[GameSessionID] bigint NOT NULL DEFAULT ((0)) ,
[CustomerID] int NOT NULL DEFAULT ((0)) ,
[GamePoints] int NOT NULL DEFAULT ((0)) ,
[HonorPoints] int NOT NULL DEFAULT ((0)) ,
[SkillPoints] int NOT NULL DEFAULT ((0)) ,
[Kills] int NOT NULL DEFAULT ((0)) ,
[Deaths] int NOT NULL DEFAULT ((0)) ,
[ShotsFired] int NOT NULL DEFAULT ((0)) ,
[ShotsHits] int NOT NULL ,
[Headshots] int NOT NULL DEFAULT ((0)) ,
[AssistKills] int NOT NULL DEFAULT ((0)) ,
[Wins] int NOT NULL DEFAULT ((0)) ,
[Losses] int NOT NULL DEFAULT ((0)) ,
[CaptureNeutralPoints] int NOT NULL DEFAULT ((0)) ,
[CaptureEnemyPoints] int NOT NULL DEFAULT ((0)) ,
[TimePlayed] int NOT NULL DEFAULT ((0)) ,
[GameReportTime] datetime NOT NULL DEFAULT (((1)/(1))/(1970)) ,
[GameDollars] int NOT NULL DEFAULT ((0)) ,
[TeamID] int NOT NULL DEFAULT ((2)) ,
[MapID] int NOT NULL DEFAULT ((255)) ,
[MapType] int NULL 
)


GO

-- ----------------------------
-- Records of DBG_UserRoundResults
-- ----------------------------
BEGIN TRANSACTION
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[DBG_WOAdminChanges]
-- ----------------------------
CREATE TABLE [dbo].[DBG_WOAdminChanges] (
[ChangeID] int NOT NULL IDENTITY(1,1) ,
[ChangeTime] datetime NULL ,
[UserName] nvarchar(64) NULL ,
[Action] int NULL ,
[Field] varchar(512) NULL ,
[RecordID] int NULL ,
[OldValue] varchar(2048) NULL ,
[NewValue] varchar(2048) NULL 
)


GO

-- ----------------------------
-- Records of DBG_WOAdminChanges
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[DBG_WOAdminChanges] ON
GO
SET IDENTITY_INSERT [dbo].[DBG_WOAdminChanges] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[DonateLogs]
-- ----------------------------
CREATE TABLE [dbo].[DonateLogs] (
[id] int NOT NULL IDENTITY(1,1) ,
[CustomerID] int NOT NULL ,
[GC] int NOT NULL ,
[Item] int NOT NULL ,
[ItemAmount] int NOT NULL ,
[Time] datetime NOT NULL ,
[CardNo] nvarchar(50) NULL 
)


GO

-- ----------------------------
-- Records of DonateLogs
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[DonateLogs] ON
GO
SET IDENTITY_INSERT [dbo].[DonateLogs] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[FinancialTransactions]
-- ----------------------------
CREATE TABLE [dbo].[FinancialTransactions] (
[CustomerID] int NOT NULL ,
[TransactionID] varchar(64) NOT NULL ,
[TransactionType] int NOT NULL ,
[DateTime] datetime NOT NULL ,
[Amount] float(53) NOT NULL ,
[ResponseCode] varchar(64) NOT NULL ,
[AprovalCode] varchar(16) NOT NULL ,
[ItemID] varchar(32) NOT NULL 
)


GO

-- ----------------------------
-- Records of FinancialTransactions
-- ----------------------------
BEGIN TRANSACTION
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[FriendsMap]
-- ----------------------------
CREATE TABLE [dbo].[FriendsMap] (
[CustomerID] int NULL ,
[FriendID] int NULL ,
[FriendStatus] int NULL ,
[DateAdded] datetime NULL 
)


GO

-- ----------------------------
-- Records of FriendsMap
-- ----------------------------
BEGIN TRANSACTION
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[HWID_Ban]
-- ----------------------------
CREATE TABLE [dbo].[HWID_Ban] (
[HardwareID] nvarchar(50) NOT NULL ,
[Reason] nvarchar(200) NOT NULL 
)


GO

-- ----------------------------
-- Records of HWID_Ban
-- ----------------------------
BEGIN TRANSACTION
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[HWID_Log]
-- ----------------------------
CREATE TABLE [dbo].[HWID_Log] (
[id] int NOT NULL ,
[CustomerID] int NOT NULL ,
[ip] nvarchar(50) NOT NULL ,
[HardwareID] nvarchar(50) NOT NULL ,
[Team] datetime NOT NULL 
)


GO

-- ----------------------------
-- Records of HWID_Log
-- ----------------------------
BEGIN TRANSACTION
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[Items_Attachments]
-- ----------------------------
CREATE TABLE [dbo].[Items_Attachments] (
[ItemID] int NOT NULL ,
[FNAME] varchar(32) NOT NULL ,
[Type] int NOT NULL ,
[Name] nvarchar(32) NOT NULL DEFAULT '' ,
[Description] nvarchar(256) NOT NULL DEFAULT '' ,
[MuzzleParticle] varchar(64) NOT NULL ,
[FireSound] varchar(256) NOT NULL ,
[Damage] float(53) NOT NULL ,
[Range] float(53) NOT NULL ,
[Firerate] float(53) NOT NULL ,
[Recoil] float(53) NOT NULL ,
[Spread] float(53) NOT NULL ,
[Clipsize] int NOT NULL ,
[ScopeMag] float(53) NOT NULL ,
[ScopeType] varchar(32) NOT NULL ,
[AnimPrefix] varchar(32) NOT NULL ,
[SpecID] int NOT NULL DEFAULT ((0)) ,
[Category] int NOT NULL DEFAULT ((19)) ,
[Price1] int NOT NULL DEFAULT ((0)) ,
[Price7] int NOT NULL DEFAULT ((0)) ,
[Price30] int NOT NULL DEFAULT ((0)) ,
[PriceP] int NOT NULL DEFAULT ((0)) ,
[GPrice1] int NOT NULL DEFAULT ((0)) ,
[GPrice7] int NOT NULL DEFAULT ((0)) ,
[GPrice30] int NOT NULL DEFAULT ((0)) ,
[GPriceP] int NOT NULL DEFAULT ((0)) ,
[IsNew] int NOT NULL DEFAULT ((0)) ,
[LevelRequired] int NOT NULL DEFAULT ((0)) ,
[Weight] int NOT NULL DEFAULT ((0)) 
)


GO

-- ----------------------------
-- Records of Items_Attachments
-- ----------------------------
BEGIN TRANSACTION
GO
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400000', N'ATTM_Grip_01', N'3', N'Forward Grip', N'The bottom rail grip will allow the shooter more control of the weapon by use of the attached grip.', N'', N'', N'0', N'0', N'0', N'0', N'-5', N'0', N'0', N'', N'Grip', N'1001', N'19', N'0', N'0', N'0', N'30', N'0', N'0', N'0', N'0', N'0', N'9', N'600');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400001', N'ATTM_Mag_AK74_30_01', N'4', N'5.45 AK 30', N'This is a standard 5.45x39mm 30 rounds clip for the russian AK 74M family of rifles', N'', N'', N'0', N'0', N'0', N'0', N'0', N'30', N'0', N'', N'', N'4200', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'700', N'0', N'0', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400003', N'ATTM_Optic_Acog_01', N'1', N'ACOG', N'The Acog is a rail mounted telescopic sight used to extend a shooters accuracy on medium to long range shots.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'50', N'acog_fps', N'ASR_Scar', N'5001', N'19', N'0', N'0', N'0', N'20', N'0', N'0', N'0', N'0', N'0', N'20', N'500');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400004', N'ATTM_Side_Laser_01', N'2', N'Rifle Laser', N'The Laser attachment mounts onto the side of a weapon and allows the shooter to mark the target with the laser.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'', N'3001', N'19', N'0', N'0', N'0', N'20', N'0', N'0', N'0', N'0', N'0', N'18', N'300');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400005', N'ATTM_Optic_Eotech_01', N'1', N'Holographic', N'The Holosight is a rail mounted holographic sight used to extend a shooters accuracy at close and medium ranges.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'35', N'eotech_fps', N'ASR_Scar', N'5001', N'19', N'0', N'0', N'0', N'30', N'0', N'0', N'0', N'0', N'0', N'15', N'500');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400006', N'ATTM_Optic_Iron_Scar', N'1', N'SCAR IS', N'This is the improved flip up iron sight system for the Scar assault rifle. It attachess onto the upper rail.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'ASR_Scar', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400007', N'ATTM_Optic_Kobra_01', N'1', N'Kobra', N'The Kobra is a rail mounted red dot sight used to extend a shooters accuracy at close and medium range targets.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'20', N'kobra_red_dot_fps', N'ASR_Scar', N'5001', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1500', N'0', N'5', N'400');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400008', N'ATTM_Optic_Scope6x_01', N'1', N'Tactical Sniper Scope', N'8x rail mounted scope designed to be used used atmedium ranges.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'90', N'psg1', N'', N'6001', N'19', N'0', N'0', N'0', N'30', N'0', N'0', N'0', N'0', N'0', N'0', N'600');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400009', N'ATTM_Grip_Mp7_01', N'3', N'SMG Grip', N'The M 7 grip provides a shooter with more control and accuracy when firing a weapon at a high rate of fire.', N'', N'', N'0', N'0', N'0', N'0', N'-5', N'0', N'0', N'', N'Grip', N'1002', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1500', N'0', N'21', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400010', N'ATTM_Mag_M4_60rnd_01', N'4', N'STANAG 60', N'Large 60 round clip of 5.45x45mm NATO rounds for use with the M4 and M16', N'', N'', N'0', N'0', N'0', N'0', N'10', N'60', N'0', N'', N'', N'4001', N'19', N'0', N'0', N'0', N'15', N'0', N'0', N'0', N'0', N'0', N'25', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400012', N'ATTM_Muzzle_FlashHider_01', N'0', N'Flash Hider', N'The Muzzle FlashHider will lower the visual flash of a weapon when fired at normal or rapid speeds.', N'muzzle_asr_noflash', N'', N'0', N'-5', N'0', N'0', N'0', N'0', N'0', N'', N'', N'2001', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'2000', N'0', N'15', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400013', N'ATTM_Muzzle_Silencer_01', N'0', N'Silencer ', N'The Silencer will lower the sound of a weapons discharge when fired at normal or rapid speeds.', N'muzzle_asr_noflash', N'Sounds/NewWeapons/SMG/HK_MP5SD', N'-20', N'-10', N'0', N'0', N'0', N'0', N'0', N'', N'', N'2001', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'3000', N'0', N'21', N'300');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400015', N'ATTM_Mag_M4_40rnd_01', N'4', N'STANAG 45', N'Medium 45 round clip of 5.45x45mm NATO rounds for use with the M4 and M16 Assault Rifles', N'', N'', N'0', N'0', N'0', N'0', N'0', N'45', N'0', N'', N'', N'4001', N'19', N'0', N'0', N'0', N'10', N'0', N'0', N'0', N'0', N'0', N'15', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400016', N'ATTM_Mag_M4_30rnd_01', N'4', N'STANAG 30', N'Standard 30 round clip of 5.45x45mm NATO rounds for use with the M4 and M16 Assault Rifles', N'', N'', N'0', N'0', N'0', N'0', N'0', N'30', N'0', N'', N'', N'4001', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1000', N'0', N'0', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400017', N'ATTM_Mag_C-Mag_01', N'4', N'STANAG C-Mag', N'Maximum size100 round drum of 5.45x45mm NATO rounds for use with the M4 and M16 Assault Rifles', N'', N'', N'0', N'0', N'0', N'0', N'20', N'100', N'0', N'', N'', N'4001', N'19', N'0', N'0', N'0', N'20', N'0', N'0', N'0', N'0', N'0', N'35', N'800');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400018', N'ATTM_Side_Flashlight_01', N'2', N'Rifle Flashlight', N'The Flashlight can be used to light dark areas and temporarily blind a shooters opponent.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'', N'3001', N'19', N'0', N'0', N'0', N'20', N'0', N'0', N'0', N'0', N'0', N'14', N'600');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400019', N'ATTM_Optic_SwissCompact_01', N'1', N'Compact Scope', N'The Swiss Compact Sight is a rail mounted scope used to extend a shooters accuracy at close and medium ranges.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'60', N'aw50', N'', N'5001', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'5000', N'0', N'40', N'600');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400020', N'ATTM_Optic_SwissRedDot_01', N'1', N'Red Dot SP', N'The Swiss Red Dot Sight is a rail mounted sight used to extend a shooters accuracy at close and medium range targets', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'35', N'swiss_red_dot_fps', N'ASR_Scar', N'5001', N'19', N'0', N'0', N'0', N'20', N'0', N'0', N'0', N'0', N'0', N'30', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400021', N'ATTM_Pistol_Laser_01', N'2', N'Pistol laser', N'The Pistol Laser is mounted on the left side of a handgun and used to target the enemy with a laser light.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'', N'3002', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1000', N'0', N'18', N'300');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400022', N'ATTM_Pistol_flashlight_WL1', N'2', N'Pistol Flashlight', N'The Pistol Flashlight can be used to light dark areas and temporarily blind a shooters opponent.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'', N'3002', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'2000', N'0', N'14', N'300');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400023', N'ATTM_Optic_Reflex_01', N'1', N'Reflex Sight', N'The Reflex Sight is a rail mounted scope used to extend a shooters accuracy at medium and long ranges.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'20', N'reflex_fps', N'ASR_Scar', N'5001', N'19', N'0', N'0', N'0', N'30', N'0', N'0', N'0', N'0', N'0', N'5', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400024', N'ATTM_Optic_Iron_M4FFH', N'1', N'M4 IS', N'This is the improved flip up iron sight system for the M4 Blackwater. It attachess onto the upper rail.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'ASR_Scar', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400025', N'ATTM_Optic_Iron_SG556', N'1', N'SIG 556 IS', N'This is the improved flip up iron sight system for the SG 555. It attachess onto the upper rail.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'ASR_Scar', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400026', N'ATTM_Optic_Iron_Mp7', N'1', N'MP7 IS', N'This is the improved flip up iron sight system for the Mp 7. It attachess onto the upper rail.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'ASR_Scar', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400027', N'ATTM_Optic_PSO1_01', N'1', N'PSO-1', N'The PSO1 is a rail mounted telescopic sight used to extend a shooters accuracy on medium to long range shots.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'70', N'pso1', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400029', N'ATTM_Mag_SG556_30rnd_01', N'4', N'G36 ammo', N'This is a 30 round magazine of 5.56x45mm specially designed for the G36 Assault Rifle', N'', N'', N'0', N'0', N'0', N'0', N'0', N'30', N'0', N'', N'', N'4100', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'500', N'0', N'0', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400030', N'ATTM_MAG_Vintorez_20', N'4', N'VSS-20', N'"Specially designed high power 9x39mm low velocity, high penetration subsonic round for VSS Vintorez sniper rifle. Extended 20 rounds magazine."', N'', N'', N'0', N'0', N'0', N'0', N'0', N'20', N'0', N'', N'', N'4002', N'19', N'0', N'0', N'0', N'35', N'0', N'0', N'0', N'0', N'0', N'40', N'500');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400031', N'ATTM_MAG_Vintorez_10', N'4', N'VSS-10', N'"Specially designed high power 9x39mm low velocity, high penetration subsonic round for VSS Vintorez sniper rifle. Standard 10 rounds magazine."', N'', N'', N'0', N'0', N'0', N'0', N'0', N'10', N'0', N'', N'', N'4002', N'19', N'0', N'0', N'0', N'20', N'0', N'0', N'0', N'0', N'0', N'0', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400032', N'ATTM_Mag_Mp7_60rnd_01', N'4', N'MP7 40', N'HK 4.6&times;30mm 40 rounds extended magazine for MP7 submachine gun.', N'', N'', N'0', N'0', N'0', N'0', N'10', N'40', N'0', N'', N'', N'4003', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1000', N'0', N'25', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400033', N'ATTM_Mag_Mp7_30rnd_01', N'4', N'MP7 30', N'HK 4.6&times;30mm 30 rounds magazine for MP7 submachine gun.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'30', N'0', N'', N'', N'4003', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'700', N'0', N'0', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400034', N'ATTM_Mag_SIGP226', N'4', N'9x19 Para Mag', N'18 rounds 9x19mm Parabellum magazine specially designed for Sig Sauer P226 handgun', N'', N'', N'0', N'0', N'0', N'0', N'0', N'18', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'250', N'1', N'0', N'400');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400035', N'ATTM_Optic_Iron_M249', N'1', N'M249 IS', N'This is the improved flip up iron sight system for the M249 machine gun. It attachess onto the upper rail.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400036', N'ATTM_Optic_Iron_Keltech', N'1', N'KT IS', N'This is the improved flip up iron sight system for the Keltech shotgun that attaches on it''s upper rail.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400038', N'ATTM_Optic_BW_LongRange_01', N'1', N'Blackwater Long Range', N'The Blackwater Long Range scope is a 10x rail mounted scope designed to be used used  at long ranges.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'120', N'sv98', N'', N'6001', N'19', N'0', N'0', N'0', N'30', N'0', N'0', N'0', N'0', N'0', N'45', N'800');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400039', N'ATTM_Optic_MultiRail_01', N'1', N'Swiss Arms Scope 8x', N'The Swiss Arms multirail scope is a rail mounted scope used to extend a shooters accuracy at medium and long ranges.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'70', N's50hs', N'', N'6001', N'19', N'0', N'0', N'0', N'20', N'0', N'0', N'0', N'0', N'0', N'25', N'700');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400040', N'ATTM_Optic_Iron_AK74M', N'1', N'Iron AK74M', N'This is the improved flip up iron sight system for the AK-74M n that attaches on it''s upper rail.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400042', N'ATTM_Optic_Iron_G36', N'1', N'Iron G36', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400043', N'ATTM_Mag_AW50_10rnd_01', N'4', N'AWM .338 Magnum ammo', N'8.6x70mm specialized rimless bottlenecked centerfire 10 rounds cartridge. Specifically designed for AWM rifle', N'', N'', N'0', N'0', N'0', N'0', N'0', N'10', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'30', N'0', N'0', N'0', N'0', N'0', N'0', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400046', N'ATTM_Mag_P90_50rnd_01', N'4', N'P90 50 rounds', N'P90 Compact Assault rifle rounds. 50 round clip', N'', N'', N'0', N'0', N'0', N'0', N'0', N'50', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'10', N'0', N'0', N'0', N'0', N'1', N'0', N'550');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400047', N'ATTM_Mag_Bizon_01', N'4', N'Bizon 64 ammo', N'Bizon sub machine gun clip. Clip contains 64 rounds of 9x18mm ammo', N'', N'', N'0', N'0', N'0', N'0', N'0', N'64', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1200', N'0', N'0', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400048', N'ATTM_Mag_SVD_10Rnd_01', N'4', N'SVD ammo', N'10 round magazine of 7.62x54mm ammo for the SVD (Dragunov Sniper Rifle)', N'', N'', N'0', N'0', N'0', N'0', N'0', N'10', N'0', N'', N'', N'4004', N'19', N'0', N'0', N'0', N'10', N'0', N'0', N'0', N'0', N'0', N'0', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400049', N'ATTM_Mag_scorpionevo3_01', N'4', N'CZ Scorpion EVO-3 ammo', N'30 rounds of 9x19mm ammo for the CZ Scorpion EVO-3 submachine gun', N'', N'', N'0', N'0', N'0', N'0', N'0', N'30', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'2000', N'0', N'0', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400050', N'ATTM_Mag_usas12_Drum_01', N'4', N'AA-12 Drum', N'12 gauge 20 rounds drum magazine for AA-12 shotgun', N'', N'', N'0', N'0', N'0', N'0', N'0', N'20', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1250', N'0', N'0', N'500');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400051', N'ATTM_Optic_Iron_ScorpionEVO3', N'1', N'EVO-3 IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400052', N'ATTM_Optic_Iron_Bizon', N'1', N'Bizon IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400053', N'ATTM_Optic_Iron_Usas12', N'1', N'USS-12 IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400054', N'ATTM_Optic_Iron_P90', N'1', N'P90 IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400055', N'ATTM_Optic_Iron_Pecheneg', N'1', N'Pecheneg IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400056', N'ATTM_Optic_Iron_PKM', N'1', N'PKM IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400058', N'ATTM_Optic_Iron_Sig516', N'1', N'SIG516 IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400059', N'ATTM_Optic_Iron_tar21', N'1', N'TAR21 IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400060', N'ATTM_Optic_Iron_RPK', N'1', N'RPK IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400061', N'ATTM_Optic_Iron_RPO', N'1', N'RPO IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400062', N'ATTM_Optic_Iron_An94', N'1', N'AN94 IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400065', N'ATTM_Optic_Iron_SUP_AT4', N'1', N'AT4 IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400066', N'ATTM_Optic_Iron_AacHoneyBadger', N'1', N'M4 IS2', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400069', N'ATTM_Mag_AN94_01', N'4', N'5.45 AK 45 ', N'This is a standard 5.45x39mm 45 rounds clip for the russian AK74 family of rifles', N'', N'', N'0', N'0', N'0', N'0', N'0', N'45', N'0', N'', N'', N'4200', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'2000', N'0', N'15', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400070', N'ATTM_Mag_AW_01', N'4', N'.308 Winchester Sniper rifle amm', N'7.62x51mm NATO sniper rifle clip for use with the .308 Winchester Sniple Rifle. Magazine holds 10 rounds', N'', N'', N'0', N'0', N'0', N'0', N'0', N'10', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'30', N'0', N'0', N'0', N'0', N'0', N'0', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400071', N'ATTM_Mag_Jericho', N'4', N'9mm Mag', N'"15 rounds magazine of 9mm ammo for use with: B92, B93R and Jericho handguns"', N'', N'', N'0', N'0', N'0', N'0', N'0', N'13', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'900', N'0', N'0', N'300');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400073', N'ATTM_Mag_Saiga_01', N'4', N'Saiga 10 ammo', N'12 gauge 10 rounds magazine for SAIGA shotgun', N'', N'', N'0', N'0', N'0', N'0', N'0', N'10', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1250', N'0', N'0', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400074', N'ATTM_Muzzle_AacHoneyBadger_01', N'0', N'standard muzzle', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'30', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400079', N'ATTM_Mag_MP5A4_01', N'4', N'MP5 10mm Mag', N'10mm Auto 30 rounds cartridge for MP5/10 submachine gun', N'', N'', N'0', N'0', N'0', N'0', N'0', N'30', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'900', N'0', N'0', N'400');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400080', N'ATTM_Optic_Iron_Saiga', N'1', N'SAIGA IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400081', N'ATTM_Optic_Iron_MP5A4', N'1', N'XA5 IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400082', N'ATTM_Optic_Iron_Mossberg590_01', N'1', N'M590 Is', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400083', N'ATTM_Optic_Iron_Sr-1_Veresk', N'1', N'Veresk IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400084', N'ATTM_Mag_Sr-1_Veresk_01', N'4', N'SMG-20 ammo', N'9x19mm Para 20 rounds magazine specially designed for Uzi and Veresk smg.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'20', N'0', N'', N'', N'4006', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1000', N'0', N'0', N'300');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400085', N'ATTM_Mag_Sr-1_Veresk_02', N'4', N'SMG-40 ammo', N'9x19mm Para 40 rounds magazine specially designed for Uzi and Veresk smg.', N'', N'', N'0', N'0', N'0', N'0', N'-10', N'40', N'0', N'', N'', N'4006', N'19', N'0', N'0', N'0', N'10', N'0', N'0', N'0', N'0', N'0', N'20', N'500');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400086', N'ATTM_Mag_Desert_Eagle_01', N'4', N'Desert Eagle ammo', N'.50 Action Express 7 round magazine for Desert Eagle handgun.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'7', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'10', N'0', N'0', N'0', N'0', N'1', N'0', N'350');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400087', N'ATTM_Mag_FN57', N'4', N'5.7 FN M240 Mag', N'FN (M240) machine gun round magazine. 20 rounds of 5.7&times;28mm ammo', N'', N'', N'0', N'0', N'0', N'0', N'0', N'20', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1250', N'0', N'0', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400088', N'ATTM_Grip_Sr-1_Veresk_01', N'3', N'Modular Aluminum Combat Grip', N'', N'', N'', N'0', N'0', N'0', N'0', N'-5', N'0', N'0', N'', N'Grip', N'1003', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1500', N'0', N'21', N'300');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400099', N'ATTM_Mag_C-Mag_01', N'4', N'G36 C-Mag', N'"This is a 5.56x45mm, 100 round drum magazine specially designed for for the G36 Assault Rifle"', N'', N'', N'0', N'0', N'0', N'0', N'20', N'100', N'0', N'', N'', N'4100', N'19', N'0', N'0', N'0', N'15', N'0', N'0', N'0', N'0', N'1', N'35', N'800');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400100', N'ATTM_Mag_AK74_Drum_01', N'4', N'5.45 AK Drum', N'100 rounds drum for AK74 and RPK machine guns', N'', N'', N'0', N'0', N'0', N'0', N'20', N'100', N'0', N'', N'', N'4200', N'19', N'0', N'0', N'0', N'15', N'0', N'0', N'0', N'0', N'0', N'35', N'800');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400101', N'ATTM_Mag_RPK_01', N'4', N'7.62 AKM clip', N'AKM Assault Rifle magazine. 40 7.62x39mm rounds', N'', N'', N'0', N'0', N'0', N'0', N'10', N'40', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1000', N'0', N'25', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400119', N'ATTM_Optic_Iron_Mossada', N'1', N'MASADA IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'10', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400121', N'ATTM_Optic_Iron_Usas12', N'1', N'USS-12 IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'10', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400127', N'ATTM_Optic_Iron_M16', N'1', N'M16 IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'10', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400128', N'ATTM_Optic_Iron_AKM', N'1', N'AKM IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'10', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400129', N'ATTM_Optic_Iron_AKS74U', N'1', N'AKS IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'10', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400133', N'ATTM_Mag_M82', N'4', N'.50 BMG', N'12.7x99mm NATO anti material rounds for M107A1 rifle. 5 rounds per magazine', N'', N'', N'0', N'0', N'0', N'0', N'0', N'5', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'25', N'0', N'0', N'0', N'0', N'0', N'0', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400134', N'Attm_Optic_Iron_UZI', N'1', N'UZI IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'10', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400135', N'ATTM_Mag_Jericho', N'4', N'.45 ACP STI Eagle Elite ammo', N'.45 ACP 10 rounds magazine for the STI Eagle Elite handgun', N'', N'', N'0', N'0', N'0', N'0', N'0', N'10', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1250', N'0', N'0', N'300');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400136', N'ATTM_Mag_Shotgun_8x', N'4', N'12 Gauge Slug', N'8 slugs for use in the Mossberg and KT Decider shotguns', N'', N'', N'0', N'0', N'0', N'0', N'0', N'8', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1250', N'0', N'0', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400137', N'ATTM_Mag_Shotgun_2x', N'4', N'2x 12gauge ', N'Ammo for use with the double barrel shotgun', N'', N'', N'0', N'0', N'0', N'0', N'0', N'2', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1250', N'0', N'0', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400139', N'ATTM_Mag_Arrow_Exp_01', N'4', N'Arrow Explosive', N'Explosive bolt (arrow) for the crossbow', N'', N'', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400140', N'ATTM_Mag_Arrow_Reg_01', N'4', N'Arrow', N'A sharp arrow for use with compound crossbows and other string drawn weapons', N'', N'', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'200');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400141', N'ATTM_Mag_Shotgun_2x', N'4', N'Shotgun shell 2x', N'2 Shells for use in the double barrel', N'', N'', N'0', N'0', N'0', N'0', N'0', N'2', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1250', N'0', N'0', N'200');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400142', N'ATTM_Mag_Shotgun_8x', N'4', N'Shotgun shell 8x', N'8 Shells for use in standard shotguns', N'', N'', N'0', N'0', N'0', N'0', N'0', N'8', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'5000', N'0', N'0', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400143', N'ATTM_Mag_M249_Box_TPS_01', N'4', N'M249 Ammo Box', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'100', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'25', N'0', N'0', N'0', N'0', N'1', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400144', N'ATTM_Mag_Ruger', N'4', N'Clip for Ruger', N'10 round, 22 cal ammo clip for Ruger pistols and rifles', N'', N'', N'0', N'0', N'0', N'0', N'0', N'10', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'700', N'0', N'0', N'300');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400145', N'ATTM_Mag_Colt_Anaconda', N'4', N'Anaconda clip', N'6 rounds for the Anaconda', N'', N'', N'0', N'0', N'0', N'0', N'0', N'6', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'6', N'0', N'0', N'0', N'0', N'0', N'0', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400146', N'ATTM_Mag_RugerASR_Big', N'4', N'Large Kruger Rifle clip', N'30 round rotary loaded mag', N'', N'', N'0', N'0', N'0', N'0', N'0', N'30', N'0', N'', N'', N'7001', N'19', N'0', N'0', N'0', N'15', N'0', N'0', N'0', N'0', N'0', N'0', N'600');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400147', N'ATTM_Mag_RugerASR_Mid', N'4', N'Medium Kruger Rifle clip', N'20 round rotary loaded magazine', N'', N'', N'0', N'0', N'0', N'0', N'0', N'20', N'0', N'', N'', N'7001', N'19', N'0', N'0', N'0', N'10', N'0', N'0', N'0', N'0', N'0', N'0', N'500');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400148', N'ATTM_Mag_RugerASR_Small', N'4', N'Standard Kruger .22 Rifle Clip', N'Standard issue 10 round rotary loaded magazine.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'10', N'0', N'', N'', N'7001', N'19', N'0', N'0', N'0', N'5', N'0', N'0', N'0', N'0', N'0', N'0', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400149', N'ATTM_Optic_Iron_RugerASR', N'1', N'Kruger Rifle IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'10', N'default', N'', N'7100', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400150', N'ATTM_Mag_Mini14', N'4', N'Kruger Mini-14 clip', N'Standard size clip for the Kruger Mini-14', N'', N'', N'0', N'0', N'0', N'0', N'0', N'10', N'0', N'', N'', N'7020', N'19', N'0', N'0', N'0', N'10', N'0', N'0', N'0', N'0', N'0', N'0', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400151', N'ATTM_Optic_Iron_Mini14', N'1', N'Kruger Mini-14 IS', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'default', N'', N'7200', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400152', N'ATTM_Mag_Flare', N'4', N'Flare clip', N'Ammo for the flare gun', N'', N'', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400153', N'ATTM_Mag_Mosin', N'4', N'Standard Mosin Magazine', N'Standard internally fed 5 round magazine designed for use in the Mosin rifle', N'', N'', N'0', N'0', N'0', N'0', N'0', N'5', N'0', N'', N'', N'7030', N'19', N'0', N'0', N'0', N'10', N'0', N'0', N'0', N'0', N'0', N'0', N'350');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400154', N'ATTM_Mag_Colt_1911', N'4', N'.40 caliber 1911 Mag', N'7 round single-stack magazine for the 1911', N'', N'', N'0', N'0', N'0', N'0', N'0', N'7', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'500', N'0', N'0', N'200');
GO
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400155', N'ATTM_Optic_Iron_Mosin', N'1', N'Mozin IS', N'Iron sights for the Mozin rifle.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'10', N'default', N'', N'5001', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400156', N'ATTM_Muzzle_Silencer_02', N'0', N'Pistol Silencer', N'Lowers the sound of pistols being fired', N'muzzle_asr_noflash', N'Sounds/NewWeapons/SMG/HK_MP5SD', N'-20', N'-10', N'0', N'0', N'0', N'0', N'0', N'', N'', N'8001', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'300');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400157', N'Attm_Mag_Nailgun', N'4', N'Nail Strip', N'Connected strip of nails for use in the Nail Gun', N'', N'', N'0', N'0', N'0', N'0', N'0', N'35', N'0', N'', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'500', N'0', N'0', N'300');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400158', N'ATTM_Optic_Iron_Famas', N'1', N'Famas IS', N'Famas IS', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'10', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400159', N'ATTM_Mag_Famas_30rnd_01', N'4', N'Famas 30 rounds', N'This is a 30 round magazine for the Famas that allows the shooter to carry more ammo.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'30', N'0', N'', N'', N'4001', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'450');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400162', N'Rocket_RPG7', N'4', N'RPG Teste', N'Thales viado.', N'', N'', N'0', N'0', N'0', N'0', N'0', N'30', N'0', N'', N'', N'4999', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1000');
INSERT INTO [dbo].[Items_Attachments] ([ItemID], [FNAME], [Type], [Name], [Description], [MuzzleParticle], [FireSound], [Damage], [Range], [Firerate], [Recoil], [Spread], [Clipsize], [ScopeMag], [ScopeType], [AnimPrefix], [SpecID], [Category], [Price1], [Price7], [Price30], [PriceP], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [IsNew], [LevelRequired], [Weight]) VALUES (N'400163', N'ATTM_Optic_Iron_RPG7', N'1', N'RPG7 IS', N'RPG7 IS', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'10', N'default', N'', N'0', N'19', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[Items_Gear]
-- ----------------------------
CREATE TABLE [dbo].[Items_Gear] (
[ItemID] int NOT NULL ,
[FNAME] varchar(64) NOT NULL DEFAULT ('ITEM000') ,
[Name] nvarchar(32) NOT NULL DEFAULT '' ,
[Description] nvarchar(256) NOT NULL DEFAULT '' ,
[Category] int NOT NULL DEFAULT ((0)) ,
[Weight] int NOT NULL DEFAULT ((0)) ,
[DamagePerc] int NOT NULL DEFAULT ((0)) ,
[DamageMax] int NOT NULL DEFAULT ((0)) ,
[Bulkiness] int NOT NULL DEFAULT ((0)) ,
[Inaccuracy] int NOT NULL DEFAULT ((0)) ,
[Stealth] int NOT NULL DEFAULT ((0)) ,
[Price1] int NOT NULL DEFAULT ((0)) ,
[Price7] int NOT NULL DEFAULT ((0)) ,
[Price30] int NOT NULL DEFAULT ((0)) ,
[PriceP] int NOT NULL DEFAULT ((0)) ,
[IsNew] int NOT NULL DEFAULT ((0)) ,
[ProtectionLevel] int NOT NULL DEFAULT ((1)) ,
[LevelRequired] int NOT NULL DEFAULT ((0)) ,
[GPrice1] int NOT NULL DEFAULT ((0)) ,
[GPrice7] int NOT NULL DEFAULT ((0)) ,
[GPrice30] int NOT NULL DEFAULT ((0)) ,
[GPriceP] int NOT NULL DEFAULT ((0)) 
)


GO

-- ----------------------------
-- Records of Items_Gear
-- ----------------------------
BEGIN TRANSACTION
GO
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20006', N'HEADHELMET', N'K. Style Helmet', N'The good ol'' Turtle Head is guaranteed to protect your brain. (Well, at least a little bit)\r\nLevel 2 Protection', N'13', N'1000', N'30', N'300', N'0', N'0', N'0', N'0', N'0', N'0', N'30', N'0', N'2', N'20', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20014', N'BODY_Sniper', N'Ghilli Suit', N'Standard issue sniper suit that let''s you become one with the earth and the bane of your enemies existence.', N'11', N'0', N'0', N'400', N'0', N'0', N'0', N'0', N'0', N'0', N'50', N'1', N'2', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20015', N'ARMOR_Rebel_Heavy', N'Custom Guerilla', N'Custom designed guerilla field gear and body armor that allows for decent protection under fast moving conditions. Level 2 Protection', N'11', N'4000', N'25', N'400', N'0', N'0', N'0', N'0', N'0', N'0', N'30', N'0', N'2', N'20', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20022', N'HGEAR_Beret', N'Beret Cover', N'This cover is a stylish military fashion statement which stands out smashingly in a combat zone!', N'13', N'300', N'0', N'300', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'3000');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20023', N'HGEAR_Boonie', N'Boonie Cover', N'This Jungle Boonie cover is high speed, great for blending in and blocking out the sun and bugs but it won''t stop bullets.', N'13', N'200', N'0', N'300', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'1500');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20025', N'HGEAR_Shadow', N'Shadow', N'This head gear was designed to not only make you look scary but also to protect your pretty face.', N'13', N'100', N'10', N'300', N'0', N'0', N'0', N'0', N'0', N'0', N'10', N'0', N'1', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20032', N'HGEAR_Mask_Black_01', N'Black Mask', N'The frightening Black Mask of Death, sure to give your enemies the chills.', N'13', N'100', N'10', N'300', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'5', N'0', N'0', N'0', N'1000');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20033', N'HGEAR_Mask_Clown_01', N'Clown Mask', N'For all of you Jokers out there, we bring you the Clown Mask.', N'13', N'100', N'10', N'300', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'10', N'0', N'0', N'0', N'1000');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20034', N'HGEAR_Mask_Hockey_01', N'Jason Mask', N'Hiding a monster, this mask strikes real horror in those who face it.', N'13', N'100', N'10', N'300', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'20', N'0', N'0', N'0', N'1000');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20035', N'HGEAR_Mask_Skull_01', N'Skull Mask', N'The Skull Mask shows you know how to Grin and Bear it!', N'13', N'100', N'10', N'300', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'30', N'0', N'0', N'0', N'1000');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20036', N'HGEAR_Mask_Slash_01', N'Slash Mask', N'While not just another pretty face, the Slash Mask is a cut above the rest! Get it and look the part of a killer.', N'13', N'100', N'10', N'300', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'40', N'0', N'0', N'0', N'1000');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20041', N'HGEAR_Boonie_Hat_Desert', N'Boonie Desert', N'This boonie hat is great for desert ops, keeping you out of the sun and foiling those pesky bugs.', N'13', N'100', N'0', N'300', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'1500');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20042', N'HGEAR_Boonie_Hat_MilGreen', N'Boonie Green', N'While not a bullet stopper this boonie hat will keep you cool and in camo in the wild places.', N'13', N'100', N'0', N'300', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'1500');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20043', N'HGEAR_M9_Helmet_Black_01', N'M9 helmet black', N'The M9 helmet gives a good standard of head protection. This one in black.', N'13', N'1000', N'25', N'300', N'0', N'0', N'0', N'0', N'0', N'0', N'25', N'0', N'1', N'5', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20046', N'HGEAR_M9_Helmet_Desert_02', N'M9 Helmet with Goggles', N'The M9 helmet gives a good standard of head protection. This one with goggles.', N'13', N'1000', N'25', N'300', N'0', N'0', N'0', N'0', N'0', N'0', N'25', N'0', N'1', N'25', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20047', N'HGEAR_M9_Helmet_MilGreen_01', N'M9 Helmet Green', N'The M9 helmet gives a good standard of head protection. This one in green.', N'13', N'1000', N'25', N'300', N'0', N'0', N'0', N'0', N'0', N'0', N'25', N'0', N'1', N'25', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20048', N'HGEAR_M9_Helmet_Urban_01', N'M9 Helmet Urban', N'The M9 helmet gives a good standard of head protection. This one in Urban camo.', N'13', N'1000', N'25', N'300', N'0', N'0', N'0', N'0', N'0', N'0', N'35', N'1', N'1', N'30', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20049', N'HGEAR_M9_Helmet_forest_01', N'M9 Helmet Forest 1', N'The M9 helmet gives a good standard of head protection. This one in a special forest pattern.', N'13', N'1000', N'25', N'300', N'0', N'0', N'0', N'0', N'0', N'0', N'25', N'0', N'1', N'35', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20050', N'HGEAR_M9_Helmet_forest_02', N'M9 Helmet Goggles 1', N'The M9 helmet gives a good standard of head protection. This one with goggles.', N'13', N'1000', N'25', N'300', N'0', N'0', N'0', N'0', N'0', N'0', N'25', N'0', N'1', N'40', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20054', N'Armor_IBA_01_Sand', N'IBA Sand', N'Special IBA release gear in Sand toned pattern with Level 2 protection.', N'11', N'2000', N'20', N'400', N'0', N'0', N'0', N'0', N'0', N'0', N'20', N'0', N'2', N'10', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20056', N'Armor_MTV_01_Forest', N'MTV Forest', N'Limited edition MTV release Forest patterned gear with Level 2 protection.', N'11', N'3000', N'20', N'400', N'0', N'0', N'0', N'0', N'0', N'0', N'25', N'0', N'2', N'15', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20057', N'HEAVYARMOR1', N'Heavy Armor', N'Limited edition Armor Protection Level 5', N'11', N'3000', N'20', N'400', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'5', N'15', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20059', N'ARMOR_Light_Forest', N'Light Gear Forest', N'Light weight Level 1 protection in a Green Forest pattern.', N'11', N'2000', N'15', N'400', N'0', N'0', N'0', N'0', N'0', N'0', N'15', N'0', N'1', N'10', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20067', N'Hgear_KStyle_NVG_01', N'NVG Goggles', N'Standard K style helmet rigged with NVG set up for night ops.', N'13', N'1000', N'25', N'300', N'0', N'0', N'0', N'0', N'0', N'0', N'40', N'0', N'2', N'20', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20096', N'HGEAR_Boonie_hat_Leather_01', N'Boonie Hat Leather', N'This is the boonie hat that comes with a bit of sophistication. It is all leather, which is bound to be a hit, but it will not stop one!', N'13', N'100', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'1500');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20097', N'HGEAR_Fireman_Helmet_01', N'Fireman Helmet', N'The traditional Firefighters helmet has been saving men for years. Give it a try if you want to be different in the battlezone.', N'13', N'500', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'1000');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20098', N'HGEAR_HardHat_01', N'Hard Hat', N'The traditional construction hard hat is just the ticket for knocking around old buildings and showing your enemies that you are tough!', N'13', N'100', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'2000');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20170', N'Zombie', N'Basic Zombie Character', N'', N'16', N'-1', N'0', N'0', N'5', N'5', N'5', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20171', N'UpperBody_Shirt_01', N'', N'', N'14', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20172', N'LowerBody_Jeans_01', N'', N'', N'14', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20173', N'Head_Bald_01', N'', N'', N'14', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20174', N'RegularGuy', N'Ex Military', N'', N'16', N'0', N'0', N'0', N'4', N'4', N'4', N'50', N'50', N'50', N'1', N'1', N'1', N'0', N'1000', N'1000', N'1000', N'1');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20175', N'gear_backpack_16slots', N'Medium Backpack', N'A medium sized backpack with plenty of space and durability.', N'12', N'0', N'0', N'0', N'18', N'15', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'3000');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20176', N'Gear_Backpack_8slots', N'Small Backpack', N'A small backpack allowing some storage for essential items and equipment.', N'12', N'0', N'0', N'0', N'12', N'10', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20177', N'HGear_GasMask_01', N'Gas Mask', N'A mask put over your face to protect from inhaling airborn pollutants and toxic gasses', N'13', N'500', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'1000');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20178', N'HGear_GasMask_02', N'Gas Mask 2', N'A mask put over your face to protect from inhaling airborn pollutants and toxic gasses', N'13', N'500', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'1500');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20179', N'Gear_Backpack_Med_01', N'Large Backpack', N'24 item backpack. Big enough for a few weeks of adventuring', N'12', N'0', N'0', N'0', N'24', N'20', N'0', N'0', N'0', N'0', N'20', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20180', N'gear_backpack_32slots', N'Military Ruck', N'The mother of all backpacks. Carry 32 items at once!', N'12', N'0', N'0', N'0', N'32', N'30', N'0', N'0', N'0', N'0', N'30', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20181', N'gear_backpack_teddybear', N'Teddy Bear backpack', N'Roman''s favorite accessory', N'12', N'0', N'0', N'0', N'12', N'10', N'0', N'0', N'0', N'0', N'10', N'1', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20182', N'MohawkGuy', N'Ex cage fighter', N'', N'16', N'0', N'0', N'0', N'4', N'4', N'4', N'100', N'100', N'100', N'500', N'1', N'1', N'0', N'1500', N'1500', N'1500', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20183', N'Zombie_Woman', N'Zombie_Woman', N'', N'16', N'-1', N'0', N'0', N'5', N'5', N'5', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20184', N'AsianGirl', N'Hunter', N'', N'16', N'0', N'0', N'0', N'4', N'4', N'4', N'0', N'0', N'0', N'500', N'1', N'1', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20185', N'Gear_Backpack_Med_01_black', N'ALICE Rucksack', N'Large backpack that fits everything you will need. The All-purpose Lightweight Individual Carrying Equipment.  Dont forget the poo tickets!', N'12', N'0', N'0', N'0', N'28', N'25', N'0', N'0', N'0', N'0', N'30', N'1', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20186', N'Zombie_man', N'Zombie man', N'', N'16', N'-1', N'0', N'0', N'5', N'5', N'5', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20187', N'HGEAR_NV_Mil_01', N'Night vision military', N'Otherwise known as NVD''s. These enhance light allowing the user to see in almost total darkness', N'13', N'1000', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'25', N'0', N'1', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20188', N'HGEAR_NV_Civ_01', N'Night vision civilian', N'Night vision for everyone, these affordable goggles allow you to see at night while remaining hidden in the darkness', N'13', N'1000', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'3000');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20189', N'CharacterMale_01', N'Male Character', N'', N'16', N'0', N'0', N'0', N'4', N'4', N'4', N'0', N'0', N'0', N'500', N'1', N'1', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20190', N'Zombie_Military', N'Zombie Military', N'', N'16', N'-1', N'0', N'0', N'5', N'5', N'5', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20191', N'Zombie_man_02', N'Zombie Man 2', N'', N'16', N'-1', N'0', N'0', N'5', N'5', N'5', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20192', N'HGear_Pumpkin_01', N'Halloween Special!', N'A unique mask to celebrate the eve of the hallows. Eat, Drink and be Scary! Wuhahahahaaa!', N'13', N'1000', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20193', N'CharacterMale_02', N'Average Joe', N'', N'16', N'0', N'0', N'0', N'4', N'4', N'4', N'0', N'0', N'0', N'500', N'1', N'1', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20194', N'CharacterFemale_01', N'CharacterFemale_01', N'', N'16', N'0', N'0', N'0', N'4', N'4', N'4', N'0', N'0', N'0', N'500', N'1', N'1', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20195', N'CharacterFemale_02', N'CharacterFemale_02', N'', N'16', N'0', N'0', N'0', N'4', N'4', N'4', N'0', N'0', N'0', N'500', N'1', N'1', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20196', N'Gear_Backpack_GameSpot', N'Gear_Backpack_GameSpot', N'', N'12', N'0', N'0', N'0', N'20', N'15', N'0', N'0', N'0', N'0', N'30', N'1', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20197', N'HGear_Santa_Beard_01', N'Christmas Special', N'A yule tide treat', N'13', N'100', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20198', N'HGear_Xmas_Elf', N'Santa''s Lil Helper', N'Makin presents for all the good girls and boys!', N'13', N'100', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20199', N'HGear_Xmas_Snowman', N'Captain Jack Frost', N'With a corncob pipe and a button nose...', N'13', N'100', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20200', N'Gear_SantaBag_01', N'Santa''s Sack', N'With presents for everyone! Well, mostly zombies. And bandits.', N'12', N'0', N'0', N'0', N'28', N'25', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20201', N'CharacterMale_Asian', N'Asian Male', N'', N'16', N'0', N'0', N'0', N'4', N'4', N'4', N'0', N'0', N'0', N'500', N'1', N'1', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Gear] ([ItemID], [FNAME], [Name], [Description], [Category], [Weight], [DamagePerc], [DamageMax], [Bulkiness], [Inaccuracy], [Stealth], [Price1], [Price7], [Price30], [PriceP], [IsNew], [ProtectionLevel], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP]) VALUES (N'20204', N'Super Zombie', N'Super Zombie', N'', N'16', N'-1', N'0', N'0', N'1', N'1', N'1', N'1', N'7', N'30', N'314', N'0', N'1', N'0', N'11', N'77', N'3030', N'3141');
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[Items_Generic]
-- ----------------------------
CREATE TABLE [dbo].[Items_Generic] (
[ItemID] int NOT NULL ,
[FNAME] varchar(32) NOT NULL DEFAULT ('Item_Generic') ,
[Category] int NOT NULL ,
[Name] nvarchar(32) NOT NULL DEFAULT '' ,
[Description] nvarchar(512) NOT NULL DEFAULT '' ,
[Price1] int NOT NULL DEFAULT ((0)) ,
[Price7] int NOT NULL DEFAULT ((0)) ,
[Price30] int NOT NULL DEFAULT ((0)) ,
[PriceP] int NOT NULL DEFAULT ((0)) ,
[IsNew] int NOT NULL DEFAULT ((0)) ,
[LevelRequired] int NOT NULL DEFAULT ((0)) ,
[GPrice1] int NOT NULL DEFAULT ((0)) ,
[GPrice7] int NOT NULL DEFAULT ((0)) ,
[GPrice30] int NOT NULL DEFAULT ((0)) ,
[GPriceP] int NOT NULL DEFAULT ((0)) ,
[Weight] int NOT NULL DEFAULT ((0)) 
)


GO

-- ----------------------------
-- Records of Items_Generic
-- ----------------------------
BEGIN TRANSACTION
GO
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301118', N'Loot_Drop_Premium_01', N'7', N'WEAP - Melee', N'spawns of War Z Weapons', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301119', N'Loot_Drop_Premium_02', N'7', N'WEAP - Civilian', N'This crate contains a variation of premium and rare gear and outfits', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301120', N'Loot_Drop_Premium_03', N'7', N'WEAP - Law', N'This crate contains a variation of prermium and rare items to give you an advantage on the battlefield', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301121', N'Loot_Drop_Reg_01', N'7', N'WEAP - Military', N'This crate contains a variation of weapons, including the chance of unlocking an Elite gun', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301122', N'Loot_Drop_Reg_02', N'7', N'WEAP - Ammo Civilian', N'This crate contains a variation of gear and outfits', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301123', N'Loot_Drop_Reg_03', N'7', N'WEAP - Ammo Military', N'This crate contains a variation of items and boosts to give you an advantage on the battlefield', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301124', N'Loot_Drop_Premium_01', N'7', N'GEAR - Armor', N'Most Epic of all loot crates, this crate contains only Elite and Epic guns. Very rare and can be only found when you kill enemy who''s 10 or more levels higher than you', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301125', N'Item_LootBox', N'7', N'GEAR - Medical', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301126', N'Item_LootBox', N'7', N'FOOD - Generic', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301127', N'Item_LootBox', N'7', N'FOOD - Advanced Meal', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301128', N'Item_LootBox', N'7', N'FOOD - GC Grade', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301129', N'Item_LootBox', N'7', N'WEAP - Ammo Law', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301130', N'Item_LootBox', N'7', N'SPAWN - Camping', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301132', N'Item_LootBox', N'7', N'SPAWN - Rural - Residential', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301133', N'Item_LootBox', N'7', N'SPAWN - Rural - Commercial', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301134', N'Item_LootBox', N'7', N'SPAWN - Rural - Law', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301135', N'Item_LootBox', N'7', N'SPAWN - Military 1', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301136', N'Item_LootBox', N'7', N'ZOMBIE- Money', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301137', N'Item_LootBox', N'7', N'GEAR - Survival', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301138', N'Item_LootBox', N'7', N'Premium LootBox', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301139', N'Item_LootBox', N'7', N'SPAWN - Craft', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301151', N'Account_ClanCreate', N'1', N'Account_ClanCreate', N'clan create item', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301152', N'Account_ClanUpg1', N'1', N'Account_ClanUpg1', N'buy price is in permanent GC ($) \r\nNOTE- number of added clan members is in **permanent GD** price', N'0', N'0', N'0', N'200', N'0', N'0', N'0', N'0', N'0', N'15', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301153', N'Account_ClanUpg2', N'1', N'Account_ClanUpg2', N'buy price is in permanent GC ($) \r\nNOTE- number of added clan members is in **permanent GD** price', N'0', N'0', N'0', N'400', N'0', N'0', N'0', N'0', N'0', N'30', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301154', N'Account_ClanUpg3', N'1', N'Account_ClanUpg3', N'buy price is in permanent GC ($) \r\nNOTE- number of added clan members is in **permanent GD** price', N'0', N'0', N'0', N'800', N'0', N'0', N'0', N'0', N'0', N'60', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301155', N'Account_ClanUpg4', N'1', N'Account_ClanUpg4', N'buy price is in permanent GC ($) \r\nNOTE- number of added clan members is in **permanent GD** price', N'0', N'0', N'0', N'1000', N'0', N'0', N'0', N'0', N'0', N'80', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301156', N'Account_ClanUpg5', N'1', N'Account_ClanUpg5', N'buy price is in permanent GC ($) \r\nNOTE- number of added clan members is in **permanent GD** price', N'0', N'0', N'0', N'1250', N'0', N'0', N'0', N'0', N'0', N'100', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301157', N'Account_ClanUpg6', N'1', N'Account_ClanUpg6', N'buy price is in permanent GC ($) \r\nNOTE- number of added clan members is in **permanent GD** price', N'0', N'0', N'0', N'1500', N'0', N'0', N'0', N'0', N'0', N'150', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301159', N'Char_Revive', N'1', N'Char_Revive', N'item for char revive before time, price is permanent GC', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0');
INSERT INTO [dbo].[Items_Generic] ([ItemID], [FNAME], [Category], [Name], [Description], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [Weight]) VALUES (N'301321', N'Gas', N'28', N'Gas', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1500', N'0');
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[Items_LootData]
-- ----------------------------
CREATE TABLE [dbo].[Items_LootData] (
[RecordID] int NOT NULL ,
[LootID] int NOT NULL ,
[Chance] float(53) NULL ,
[ItemID] int NULL ,
[GDMin] int NULL ,
[GDMax] int NULL 
)


GO

-- ----------------------------
-- Records of Items_LootData
-- ----------------------------
BEGIN TRANSACTION
GO
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3419', N'301118', N'50', N'101267', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3420', N'301118', N'30', N'101309', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3421', N'301118', N'20', N'101308', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3422', N'301118', N'300', N'101307', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3423', N'301118', N'850', N'-1', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3424', N'301118', N'500', N'101278', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4254', N'301121', N'0.2', N'101088', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4255', N'301121', N'30', N'101063', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4256', N'301121', N'30', N'101103', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4257', N'301121', N'30', N'101022', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4258', N'301121', N'30', N'101064', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4259', N'301121', N'20', N'101112', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4260', N'301121', N'15', N'101055', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4261', N'301121', N'15', N'101002', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4262', N'301121', N'20', N'101120', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4263', N'301121', N'20', N'101004', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4264', N'301121', N'0.2', N'101087', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4265', N'301121', N'0.2', N'101247', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4266', N'301121', N'0.2', N'101084', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4267', N'301121', N'0.2', N'101085', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4268', N'301121', N'150', N'-1', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3159', N'301119', N'20', N'101032', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3160', N'301119', N'20', N'101098', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3161', N'301119', N'20', N'101111', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3162', N'301119', N'20', N'101040', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3163', N'301119', N'20', N'101158', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3164', N'301119', N'20', N'101115', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3165', N'301119', N'80', N'-1', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4243', N'301120', N'15', N'101172', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4244', N'301120', N'0.001', N'101247', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4245', N'301120', N'15', N'101103', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4246', N'301120', N'20', N'101022', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4247', N'301120', N'20', N'101200', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4248', N'301120', N'15', N'101002', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4249', N'301120', N'20', N'101111', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4250', N'301120', N'100', N'-1', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2975', N'301122', N'50', N'-1', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2976', N'301122', N'30', N'400136', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2977', N'301122', N'30', N'400137', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2978', N'301122', N'10', N'400101', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2979', N'301122', N'10', N'400016', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2980', N'301122', N'5', N'400073', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2981', N'301122', N'2', N'400070', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2982', N'301122', N'2', N'400017', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2983', N'301122', N'2', N'400048', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2984', N'301122', N'1', N'400043', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2985', N'301122', N'1', N'400071', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3136', N'301124', N'10', N'20056', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3137', N'301124', N'3', N'20015', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3138', N'301124', N'5', N'20043', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3139', N'301124', N'7', N'20047', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3140', N'301124', N'6', N'20048', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3971', N'301125', N'1', N'101304', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3972', N'301125', N'7', N'101302', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3973', N'301125', N'1.5', N'101262', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3974', N'301125', N'2', N'101256', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3975', N'301125', N'5', N'101300', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3976', N'301125', N'10', N'101261', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3977', N'301125', N'20', N'-1', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3125', N'301127', N'10', N'101295', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3126', N'301127', N'20', N'101286', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3127', N'301127', N'30', N'101298', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3128', N'301127', N'60', N'101297', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3129', N'301127', N'100', N'101290', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3130', N'301127', N'100', N'101291', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3131', N'301127', N'100', N'101292', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3132', N'301127', N'100', N'101293', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3133', N'301127', N'100', N'101299', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3134', N'301127', N'150', N'101285', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3135', N'301127', N'230', N'-1', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3111', N'301128', N'3', N'101284', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3112', N'301128', N'15', N'101295', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3113', N'301128', N'50', N'-1', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3141', N'301124', N'8', N'20096', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3142', N'301124', N'9', N'20097', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3143', N'301124', N'4', N'20098', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3144', N'301124', N'11', N'20177', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3145', N'301124', N'15', N'20178', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3146', N'301124', N'30', N'20023', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3147', N'301124', N'17', N'20032', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3148', N'301124', N'15', N'20035', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3149', N'301124', N'21', N'20041', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3150', N'301124', N'25', N'20042', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3151', N'301124', N'100', N'-1', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3034', N'301126', N'150', N'-1', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3035', N'301126', N'110', N'101296', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3036', N'301126', N'100', N'101285', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3037', N'301126', N'100', N'101283', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3038', N'301126', N'100', N'101288', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3039', N'301126', N'100', N'101289', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3040', N'301126', N'50', N'101299', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3041', N'301126', N'50', N'101294', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3042', N'301126', N'50', N'101297', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3043', N'301126', N'50', N'101286', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3044', N'301126', N'30', N'101290', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3045', N'301126', N'25', N'101292', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3046', N'301126', N'25', N'101291', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3047', N'301126', N'20', N'101293', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3048', N'301126', N'15', N'101298', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2306', N'301129', N'1', N'400070', N'0', N'0');
GO
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2307', N'301129', N'5', N'400050', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2308', N'301129', N'5', N'400079', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2309', N'301129', N'2', N'400017', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2310', N'301129', N'2', N'400015', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2311', N'301129', N'5', N'400016', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3903', N'301130', N'50', N'301118', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3904', N'301130', N'50', N'301137', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3905', N'301130', N'30', N'301125', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3906', N'301130', N'50', N'301126', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3907', N'301132', N'50', N'301119', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3908', N'301132', N'50', N'301122', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3909', N'301132', N'10', N'301137', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3910', N'301132', N'20', N'301125', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3911', N'301132', N'50', N'301126', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3912', N'301133', N'20', N'301119', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3913', N'301133', N'15', N'301122', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3914', N'301133', N'30', N'301124', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3915', N'301133', N'30', N'301126', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3916', N'301133', N'30', N'301137', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3917', N'301133', N'100', N'301125', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3918', N'301134', N'50', N'301120', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3919', N'301134', N'50', N'301129', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3920', N'301134', N'60', N'301128', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3921', N'301134', N'100', N'301124', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3922', N'301134', N'100', N'301125', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3923', N'301135', N'50', N'301123', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3924', N'301135', N'50', N'301121', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3925', N'301135', N'30', N'301124', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3926', N'301135', N'20', N'301127', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3927', N'301135', N'100', N'301125', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'302', N'301136', N'2', N'0', N'50', N'150');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'303', N'301136', N'1', N'-1', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3963', N'301137', N'20', N'20179', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3964', N'301137', N'40', N'20175', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3965', N'301137', N'70', N'101307', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3966', N'301137', N'20', N'101315', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3967', N'301137', N'100', N'101306', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3968', N'301137', N'200', N'-1', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3969', N'301137', N'15', N'20185', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3970', N'301137', N'10', N'20180', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4251', N'301120', N'4', N'101210', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4252', N'301120', N'5', N'101173', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2312', N'301129', N'5', N'400071', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2313', N'301129', N'10', N'-1', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2907', N'301123', N'15', N'400000', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2908', N'301123', N'200', N'-1', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2449', N'301139', N'100', N'301335', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2450', N'301139', N'100', N'301370', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2451', N'301139', N'100', N'301319', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'4253', N'301120', N'5', N'101169', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2314', N'301129', N'10', N'301139', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'3049', N'301126', N'95', N'301139', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2452', N'301139', N'100', N'301328', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2453', N'301139', N'100', N'301339', N'0', N'0');
INSERT INTO [dbo].[Items_LootData] ([RecordID], [LootID], [Chance], [ItemID], [GDMin], [GDMax]) VALUES (N'2454', N'301139', N'555555555', N'-1', N'0', N'0');
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[Items_Weapons]
-- ----------------------------
CREATE TABLE [dbo].[Items_Weapons] (
[ItemID] int NOT NULL ,
[FNAME] varchar(32) NOT NULL DEFAULT ('ITEM000') ,
[Category] int NOT NULL DEFAULT ((0)) ,
[Name] nvarchar(32) NOT NULL DEFAULT '' ,
[Description] nvarchar(256) NOT NULL DEFAULT '' ,
[MuzzleOffset] varchar(32) NOT NULL DEFAULT ('0 0 0') ,
[MuzzleParticle] varchar(32) NOT NULL DEFAULT ('default') ,
[Animation] varchar(32) NOT NULL DEFAULT ('assault') ,
[BulletID] varchar(32) NOT NULL DEFAULT ((5.45)) ,
[Sound_Shot] varchar(255) NOT NULL DEFAULT ('Sounds/Weapons/AK74_7_62_Shot') ,
[Sound_Reload] varchar(255) NOT NULL DEFAULT ('Sounds/Weapons/AK74_Reload') ,
[Damage] float(53) NOT NULL DEFAULT ((20)) ,
[isImmediate] int NOT NULL DEFAULT ((1)) ,
[Mass] float(53) NOT NULL DEFAULT ((0.1)) ,
[Speed] float(53) NOT NULL DEFAULT ((300)) ,
[DamageDecay] float(53) NOT NULL DEFAULT ((0)) ,
[Area] float(53) NOT NULL DEFAULT ((0)) ,
[Delay] float(53) NOT NULL DEFAULT ((0)) ,
[Timeout] float(53) NOT NULL DEFAULT ((0)) ,
[NumClips] int NOT NULL DEFAULT ((10)) ,
[Clipsize] int NOT NULL DEFAULT ((30)) ,
[ReloadTime] float(53) NOT NULL DEFAULT ((2.5)) ,
[ActiveReloadTick] float(53) NOT NULL DEFAULT ((1.2)) ,
[RateOfFire] int NOT NULL DEFAULT ((600)) ,
[Spread] float(53) NOT NULL DEFAULT ((0.08)) ,
[Recoil] float(53) NOT NULL DEFAULT ((1)) ,
[NumGrenades] int NOT NULL DEFAULT ((0)) ,
[GrenadeName] varchar(32) NOT NULL DEFAULT ('asr_grenade') ,
[Firemode] varchar(3) NOT NULL DEFAULT ((101)) ,
[DetectionRadius] int NOT NULL DEFAULT ((30)) ,
[ScopeType] varchar(32) NOT NULL DEFAULT ('default') ,
[ScopeZoom] int NOT NULL DEFAULT ((0)) ,
[Price1] int NOT NULL DEFAULT ((0)) ,
[Price7] int NOT NULL DEFAULT ((0)) ,
[Price30] int NOT NULL DEFAULT ((0)) ,
[PriceP] int NOT NULL DEFAULT ((0)) ,
[IsNew] int NOT NULL DEFAULT ((0)) ,
[LevelRequired] int NOT NULL DEFAULT ((0)) ,
[GPrice1] int NOT NULL DEFAULT ((0)) ,
[GPrice7] int NOT NULL DEFAULT ((0)) ,
[GPrice30] int NOT NULL DEFAULT ((0)) ,
[GPriceP] int NOT NULL DEFAULT ((0)) ,
[ShotsFired] bigint NOT NULL DEFAULT ((0)) ,
[ShotsHits] bigint NOT NULL DEFAULT ((0)) ,
[KillsCQ] int NOT NULL DEFAULT ((0)) ,
[KillsDM] int NOT NULL DEFAULT ((0)) ,
[KillsSB] int NOT NULL DEFAULT ((0)) ,
[IsUpgradeable] int NOT NULL DEFAULT ((1)) ,
[IsFPS] int NOT NULL DEFAULT ((0)) ,
[FPSSpec0] int NOT NULL DEFAULT ((0)) ,
[FPSSpec1] int NOT NULL DEFAULT ((0)) ,
[FPSSpec2] int NOT NULL DEFAULT ((0)) ,
[FPSSpec3] int NOT NULL DEFAULT ((0)) ,
[FPSSpec4] int NOT NULL DEFAULT ((0)) ,
[FPSSpec5] int NOT NULL DEFAULT ((0)) ,
[FPSSpec6] int NOT NULL DEFAULT ((0)) ,
[FPSSpec7] int NOT NULL DEFAULT ((0)) ,
[FPSSpec8] int NOT NULL DEFAULT ((0)) ,
[FPSAttach0] int NOT NULL DEFAULT ((0)) ,
[FPSAttach1] int NOT NULL DEFAULT ((0)) ,
[FPSAttach2] int NOT NULL DEFAULT ((0)) ,
[FPSAttach3] int NOT NULL DEFAULT ((0)) ,
[FPSAttach4] int NOT NULL DEFAULT ((0)) ,
[FPSAttach5] int NOT NULL DEFAULT ((0)) ,
[FPSAttach6] int NOT NULL DEFAULT ((0)) ,
[FPSAttach7] int NOT NULL DEFAULT ((0)) ,
[FPSAttach8] int NOT NULL DEFAULT ((0)) ,
[AnimPrefix] varchar(32) NOT NULL DEFAULT '' ,
[Weight] int NOT NULL DEFAULT ((0)) 
)


GO

-- ----------------------------
-- Records of Items_Weapons
-- ----------------------------
BEGIN TRANSACTION
GO
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'100999', N'SUP_RPG7', N'20', N'RPG-7', N'RPG-7 Mete Explosao na bunda do Thales', N'0 0 0', N'muzzle_rpg', N'rpg', N'rocket', N'Sounds/NewWeapons/SUP/SUP_Generic_Shot', N'Sounds/Weapons/New Reloads/SUP_Generic_Reload', N'200', N'0', N'0.15', N'40', N'500', N'6', N'0', N'0', N'0', N'1', N'4', N'2.43', N'30', N'2', N'10', N'0', N'asr_grenade', N'100', N'30', N'default', N'30', N'0', N'0', N'0', N'0', N'0', N'39', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'1001', N'4001', N'0', N'0', N'0', N'0', N'0', N'400163', N'0', N'0', N'400162', N'0', N'0', N'0', N'0', N'SUP_RPG7', N'3300');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101001', N'ASR_Famas', N'20', N'Famas', N'The Famas rifle is a gas operated standard infantry assault rifle, chambered for 5.56x45mm NATO round.', N'0 0 0', N'muzzle_asr', N'assault', N'5.56', N'Sounds/NewWeapons/Assault/Famas_F1', N'Sounds/Weapons/New Reloads/G36-Reload', N'33', N'0', N'1', N'300', N'300', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'700', N'0', N'1', N'0', N'asr_grenade', N'101', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'39', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'1001', N'4001', N'0', N'0', N'0', N'0', N'0', N'400158', N'0', N'0', N'400159', N'0', N'0', N'0', N'0', N'ASR_Famas', N'3300');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101002', N'ASR_M16', N'20', N'M16', N'The M16 rifle is a gas operated standard infantry assault rifle, chambered for 5.56x45mm NATO round.', N'0 0 0', N'muzzle_asr', N'assault', N'5.56', N'Sounds/NewWeapons/Assault/ColtM16', N'Sounds/Weapons/New Reloads/M16-Reload', N'27', N'0', N'1', N'500', N'300', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'625', N'9', N'7', N'0', N'asr_grenade', N'101', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'4000', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'1001', N'4001', N'0', N'0', N'0', N'0', N'0', N'400127', N'0', N'0', N'400016', N'0', N'0', N'0', N'0', N'ASR_M16', N'4000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101004', N'HG_FN57', N'25', N'FN FiveSeven', N'The FN Five Seven is a standard issue handgun that was designed to fire a 5.7 mm round. It is an effective weapon against close range targets.', N'0 0 0', N'muzzle_hg', N'pistol', N'5.45', N'Sounds/NewWeapons/Handgun/Glock9mm', N'Sounds/Weapons/HG/HG_Generic_Reload', N'32', N'0', N'1', N'500', N'35', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'800', N'8', N'3', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'8001', N'0', N'3002', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400087', N'0', N'0', N'0', N'0', N'HG_FN57', N'750');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101005', N'ASR_G36', N'20', N'G36', N'The MTAC X36 assault rifle fires a 5.56 mm round. Its high rate of fire and medium recoil deliver a reasonably tight spread on impact of targets up to medium ranges.', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'Sounds/NewWeapons/Assault/HK_G36', N'Sounds/Weapons/New Reloads/G36-Reload', N'30', N'0', N'1', N'500', N'300', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'650', N'9', N'8', N'0', N'asr_grenade', N'111', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'23', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'1001', N'4100', N'0', N'0', N'0', N'0', N'0', N'400042', N'0', N'0', N'400029', N'0', N'0', N'0', N'0', N'ASR_G36', N'3300');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101022', N'ASR_AK74M', N'20', N'AK-74M', N'The AK-74M assault rifle is an upgrade of the AK 74 design with a slightly increased rate of fire. It fires a 5.45 x 39 mm round accurately at medium ranges.', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'Sounds/NewWeapons/Assault/Kalashnikov_AK74', N'Sounds/Weapons/ASR/ASR_Generic_Reload', N'32', N'0', N'1', N'500', N'300', N'0', N'0', N'0', N'0', N'1', N'3', N'0', N'625', N'12', N'16', N'0', N'asr_grenade', N'101', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'20', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'1001', N'4200', N'0', N'0', N'0', N'0', N'0', N'400040', N'0', N'0', N'400001', N'0', N'0', N'0', N'0', N'ASR_AK74m', N'3400');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101027', N'ASR_AK74M_Elite', N'20', N'AK-74M ELITE', N'This LIMITED EDITION', N'0 0 0', N'default', N'assault', N'5.45', N'Sounds/NewWeapons/Assault/Kalashnikov_AK74', N'Sounds/Weapons/ASR/ASR_Generic_Reload', N'200', N'1', N'10', N'300', N'90', N'0', N'0', N'0', N'0', N'30', N'2.5', N'0.43', N'900', N'1', N'1', N'0', N'asr_grenade', N'011', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'1001', N'4001', N'30100', N'20100', N'10100', N'0', N'400014', N'400040', N'0', N'0', N'400001', N'0', N'0', N'0', N'0', N'ASR_AK74m', N'1000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101032', N'ASR_AKM', N'20', N'AKM', N'The AKM is an assault rifle that was designed as a simplified version of the AK-47 rifle. It fires a 7.62 x 39 mm round and is relatively effective at medium ranges.', N'0 0 0', N'muzzle_asr', N'assault', N'7.62', N'Sounds/NewWeapons/Assault/Kalashnikov_AKM', N'Sounds/Weapons/ASR/ASR_Generic_Reload', N'34', N'0', N'1', N'500', N'300', N'0', N'0', N'0', N'0', N'1', N'3', N'0', N'600', N'14', N'20', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'10', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'1001', N'0', N'0', N'0', N'0', N'0', N'0', N'400128', N'0', N'0', N'400101', N'0', N'0', N'0', N'0', N'ASR_AKM', N'3000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101033', N'asr_akm_elite', N'20', N'AKM ELITE', N'This LIMITED EDITION', N'0 0 0', N'default', N'assault', N'5.45', N'Sounds/NewWeapons/Assault/Kalashnikov_AK74', N'Sounds/Weapons/ASR/ASR_Generic_Reload', N'125', N'1', N'10', N'300', N'90', N'0', N'0', N'0', N'0', N'30', N'2.5', N'0.43', N'900', N'1', N'1', N'0', N'asr_grenade', N'011', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'1001', N'4001', N'30100', N'20100', N'10100', N'0', N'400014', N'400040', N'0', N'0', N'400001', N'0', N'0', N'0', N'0', N'ASR_AKM', N'1000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101035', N'ASR_AKS74U', N'26', N'AKS-74U', N'The AKS-74U is a shortened gas operated carbine version of the AKS-74 assault rifle. It fires a 5.45 x 39 mm round and is very effective at medium ranges.', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'Sounds/NewWeapons/Assault/AKS-74U', N'Sounds/Weapons/ASR/ASR_Generic_Reload', N'27', N'0', N'1', N'500', N'55', N'0', N'0', N'0', N'0', N'1', N'3', N'0', N'650', N'12', N'5', N'0', N'asr_grenade', N'101', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'32', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'1001', N'4200', N'0', N'0', N'0', N'0', N'0', N'400129', N'0', N'0', N'400001', N'0', N'0', N'0', N'0', N'ASR_AKS74U', N'2700');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101040', N'ASR_SIG516', N'20', N'M4 Semi', N'The civilian M4 semi auto rifle, chambered for 5.56x45mm NATO round.', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'Sounds/NewWeapons/Assault/HK_416', N'Sounds/Weapons/New Reloads/M16-Reload', N'35', N'1', N'1', N'500', N'300', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'650', N'9', N'9', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'46', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'1001', N'4001', N'0', N'0', N'0', N'0', N'0', N'400058', N'0', N'0', N'400016', N'0', N'0', N'0', N'0', N'ASR_SIG516', N'3000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101055', N'ASR_M4FFH', N'20', N'M4', N'The M4 is a versitile, light weight, commando style assault rifle that is gas operated and fires a 5.56 x 45 mm round effectively in both close and medium ranges.', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'Sounds/NewWeapons/Assault/ColtM4', N'Sounds/Weapons/New Reloads/M16-Reload', N'34', N'0', N'1', N'500', N'300', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'625', N'9', N'12', N'0', N'asr_grenade', N'111', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'35', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'1001', N'4001', N'0', N'0', N'0', N'0', N'0', N'400024', N'0', N'0', N'400016', N'0', N'0', N'0', N'0', N'ASR_M4FFH', N'3000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101060', N'MG_PKM', N'23', N'PKM', N'The PKM machine gun was designed to fire a 7.62 mm rounds from a linked system and is reasonably accurate out to medium ranged targets.', N'0 0 0', N'muzzle_asr', N'assault', N'7.62', N'Sounds/NewWeapons/LightMachineGun/Kalashnikov_PKM', N'Sounds/Weapons/MG/MG_Generic_Reload', N'36', N'0', N'1', N'600', N'300', N'0', N'0', N'0', N'0', N'1', N'5', N'2', N'625', N'15', N'10', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'34', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'5001', N'3001', N'1001', N'0', N'0', N'0', N'0', N'0', N'0', N'400056', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'MG_PKM', N'8000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101063', N'SMG_MP7', N'26', N'MP7', N'The RA TR7 submachine gun is a special issue weapon that fires a 4.6 mm standard cartridge from a 30 round magazine. It is effective at close range targets.', N'0 0 0', N'muzzle_asr', N'smg', N'5.45', N'Sounds/NewWeapons/SMG/HK_UMP', N'Sounds/Weapons/New Reloads/M7-Reload', N'24', N'0', N'1', N'500', N'35', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'800', N'11', N'6', N'0', N'asr_grenade', N'101', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'32', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'1002', N'4003', N'0', N'0', N'0', N'0', N'0', N'400026', N'0', N'0', N'400033', N'0', N'0', N'0', N'0', N'SMG_MP7', N'2000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101064', N'SMG_UZI', N'26', N'UZI', N'The special issue UZI submachine gun is a open bolt, blow back design that fires 9 mm rounds from its magazine housed in the weapons pistol grip.', N'0 0 0', N'muzzle_asr', N'smg', N'5.45', N'Sounds/NewWeapons/SMG/Uzi', N'Sounds/Weapons/New Reloads/Uzi-Reload', N'26', N'0', N'1', N'500', N'35', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'700', N'15', N'9', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'0', N'3001', N'0', N'4006', N'0', N'0', N'0', N'0', N'0', N'400134', N'0', N'0', N'400084', N'0', N'0', N'0', N'0', N'SMG_UZI', N'3500');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101068', N'SNP_SVD_PSO', N'21', N'SVD ', N'The SVD sniper rifle was designed to fire a 7.62 mm round from a 10 round clip. Combined with an Optical Sniper Scope it is extremely accurate.', N'0 0 0', N'muzzle_asr', N'assault', N'Sniper5.45', N'Sounds/NewWeapons/Sniper/SV98', N'Sounds/Weapons/New Reloads/SVD-Reload', N'70', N'0', N'1', N'800', N'600', N'0', N'0', N'0', N'0', N'1', N'3', N'1', N'80', N'3', N'3', N'0', N'asr_grenade', N'100', N'30', N'pso1', N'70', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'6001', N'0', N'0', N'4004', N'0', N'0', N'0', N'0', N'0', N'400027', N'0', N'0', N'400048', N'0', N'0', N'0', N'0', N'SNP_SVD_PSO', N'5000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101084', N'SNP_Vintorez', N'21', N'VSS VINTOREZ', N'The VSS Vintorez sniper rifle was designed to fire a 9 x 39 mm round using a gas operated, rotating bolt firing system. It is designed to be effective at medium distances.', N'0 0 0', N'muzzle_asr', N'assault', N'Sniper5.45', N'Sounds/NewWeapons/Sniper/VSS_Vintorez', N'Sounds/Weapons/ASR/ASR_Generic_Reload', N'125', N'0', N'2', N'700', N'200', N'0', N'0', N'0', N'0', N'1', N'3', N'1', N'200', N'2', N'5', N'0', N'asr_grenade', N'101', N'30', N'pso1', N'60', N'0', N'0', N'0', N'0', N'1', N'45', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'6001', N'3001', N'0', N'4002', N'0', N'0', N'0', N'0', N'0', N'400027', N'0', N'0', N'400031', N'0', N'0', N'0', N'0', N'SNP_Vintorez', N'2500');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101085', N'SNP_AW', N'21', N'MAUSER SP66', N'The Mauser SP66 sniper rifle was designed to fire a .308W round from a 5 round clip. It is highly accurate at long ranges when combined with a good scope.', N'0 0 0', N'muzzle_asr', N'assault', N'Sniper5.45', N'Sounds/NewWeapons/Sniper/SNP_AW', N'Sounds/Weapons/New Reloads/SNP_AW_reload', N'105', N'0', N'1', N'1000', N'600', N'0', N'0', N'0', N'0', N'1', N'3', N'1', N'60', N'3', N'3', N'0', N'asr_grenade', N'100', N'30', N'aw50', N'80', N'0', N'0', N'0', N'0', N'1', N'20', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'6001', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400008', N'0', N'0', N'400070', N'0', N'0', N'0', N'0', N'SNP_AW', N'6000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101087', N'SNP_AW50', N'21', N'AW Magnum', N'The AWM sniper rifle fires a custom .338W magnum round with devastating accuracy and effect. Each of its rounds deal a very high amount of damage.', N'0 0 0', N'muzzle_asr', N'assault', N'Sniper5.45', N'Sounds/NewWeapons/Sniper/SNP_AW50', N'Sounds/Weapons/New Reloads/SNP_AW_reload', N'180', N'0', N'2', N'800', N'1000', N'0', N'0', N'0', N'0', N'1', N'3', N'1', N'30', N'3', N'4', N'0', N'asr_grenade', N'100', N'30', N's50hs', N'90', N'0', N'0', N'0', N'0', N'1', N'32', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'6001', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400008', N'0', N'0', N'400043', N'0', N'0', N'0', N'0', N'SNP_AW50', N'6500');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101088', N'SNP_M82', N'21', N'M107', N'The M107  sniper rifle fires a .50 caliber round that strikes with deadly effect at extreme ranges. It has a 10 round clip that allows for a high rate of fire.', N'0 0 0', N'muzzle_asr', N'assault', N'Sniper5.45', N'Sounds/NewWeapons/Sniper/0.50Caliber', N'Sounds/Weapons/New Reloads/M107-Reload', N'150', N'0', N'2', N'800', N'900', N'0', N'0', N'0', N'0', N'1', N'3', N'1', N'60', N'5', N'7', N'0', N'asr_grenade', N'100', N'30', N'sniper', N'110', N'0', N'0', N'0', N'0', N'1', N'40', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'6001', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400008', N'0', N'0', N'400133', N'0', N'0', N'0', N'0', N'SNP_M82', N'11000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101093', N'MG_RPK', N'23', N'RPK-74 ', N'The RPK-74 machine gun fires a 5.45 mm high velocity round that delivers a reasonably accurate rate of fire onto targets out to medium ranges.', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'Sounds/NewWeapons/LightMachineGun/Kalashnikov_RPK74', N'Sounds/Weapons/New Reloads/MG_RPK_reload', N'34', N'0', N'1', N'600', N'300', N'0', N'0', N'0', N'0', N'1', N'4', N'2', N'600', N'14', N'10', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'5001', N'3001', N'1001', N'4200', N'0', N'0', N'0', N'0', N'0', N'400060', N'0', N'0', N'400100', N'0', N'0', N'0', N'0', N'MG_RPK', N'5000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101095', N'MG_M249_SAW', N'23', N'FN M249 ', N'The FN M249 machine gun fires a 5.56 mm round from a belt fed system. It is highly effective against targets at close and medium ranges.', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'Sounds/NewWeapons/LightMachineGun/FN_M249', N'Sounds/Weapons/New Reloads/MG_M240_SAW_Reload', N'31', N'0', N'1', N'600', N'300', N'0', N'0', N'0', N'0', N'1', N'6', N'2', N'600', N'14', N'5', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'25', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'5001', N'3001', N'1001', N'0', N'0', N'0', N'0', N'0', N'0', N'400035', N'0', N'0', N'400143', N'0', N'0', N'0', N'0', N'MG_M249', N'10000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101098', N'SHG_Saiga', N'22', N'SAIGA', N'The Saiga was developed as a 12 gauge shotgun with a 8 round storage tube. It inflicts a substancial amount of damage on targets at a close range.', N'0 0 0', N'muzzle_sh1', N'assault', N'buckshot', N'Sounds/NewWeapons/Shotgun/Shotgun', N'Sounds/Weapons/SHOTGUN/SHG_Generic_Reload', N'27', N'0', N'1', N'300', N'10', N'0', N'0', N'0', N'0', N'1', N'3', N'1', N'100', N'20', N'15', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'25', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'5001', N'3001', N'1001', N'0', N'0', N'0', N'0', N'0', N'0', N'400080', N'0', N'0', N'400073', N'0', N'0', N'0', N'0', N'SHG_SAIGA', N'3500');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101103', N'SMG_MP5A4', N'26', N'MP5/10', N'The MP5/10 submachine gun was designed to fire a 10 mm Auto rounds from a 30 round clip effectively against close ranged targets.', N'0 0 0', N'muzzle_asr', N'smg', N'5.45', N'Sounds/NewWeapons/SMG/HK_MP5', N'Sounds/Weapons/New Reloads/G36-Reload', N'23', N'0', N'1', N'500', N'35', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'700', N'11', N'7', N'0', N'asr_grenade', N'111', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400081', N'0', N'0', N'400079', N'0', N'0', N'0', N'0', N'SMG_MP5A4', N'2800');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101106', N'SMG_AacHoneyBadger_01', N'26', N'Honey Badger', N'The Honey Badger submachine gun fires a .300 round at a medium rate of fire causing a good amount of damage on close range targets.', N'0 0 0', N'muzzle_asr', N'smg', N'5.45', N'Sounds/NewWeapons/SMG/HK_MP5SD', N'Sounds/Weapons/New Reloads/M16-Reload', N'26', N'0', N'1', N'500', N'45', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'780', N'12', N'7', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'52', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'5001', N'3001', N'1001', N'4001', N'0', N'0', N'0', N'0', N'400074', N'400066', N'0', N'0', N'400016', N'0', N'0', N'0', N'0', N'SMG_AacHoneyBadger_01', N'4000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101107', N'SMG_P90', N'26', N'FN P90 ', N'The FN P90 submachine gun was designed to fire 9 mm rounds from a large 50 round magazine. It is very effective on close range targets.', N'0 0 0', N'muzzle_asr', N'smg', N'5.45', N'Sounds/NewWeapons/SMG/FN_P90', N'Sounds/Weapons/New Reloads/G36-Reload', N'26', N'0', N'1', N'500', N'35', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'900', N'10', N'8', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'36', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400054', N'0', N'0', N'400046', N'0', N'0', N'0', N'0', N'smg_p90', N'3000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101108', N'SMG_ScorpionEVO3', N'26', N'EVO-3', N'The EVO-3 submachine gun was designed to effectively fire 9 mm rounds from a 25 round magazine at close ranged targets at a very high rate of fire..', N'0 0 0', N'muzzle_asr', N'smg', N'5.45', N'Sounds/NewWeapons/SMG/Scorpion_EVO3', N'Sounds/Weapons/New Reloads/G36-Reload', N'26', N'0', N'1', N'500', N'35', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'800', N'11', N'10', N'0', N'asr_grenade', N'101', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'24', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'1001', N'0', N'0', N'0', N'0', N'0', N'0', N'400051', N'0', N'0', N'400049', N'0', N'0', N'0', N'0', N'SMG_ScorpionEVO3', N'3250');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101109', N'SMG_Bizon', N'26', N'BIZON', N'The Bizon submachine gun was designed to fire a 9 mm round from a 64 round magazine delivering accurate fire on close range targets.', N'0 0 0', N'muzzle_asr', N'smg', N'5.45', N'Sounds/NewWeapons/SMG/Bizon', N'Sounds/Weapons/New Reloads/G36-Reload', N'26', N'0', N'1', N'500', N'35', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'900', N'11', N'8', N'0', N'asr_grenade', N'101', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'43', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400052', N'0', N'0', N'400047', N'0', N'0', N'0', N'0', N'SMG_Bizon', N'2000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101111', N'HG_Beretta92F', N'25', N'B92', N'The B92 handgun was specially designed to fire a 9mm round and is reasonably effective agains close range targets.', N'0 0 0', N'muzzle_hg', N'pistol', N'5.45', N'Sounds/NewWeapons/Handgun/Beretta92', N'Sounds/Weapons/HG/HG_Generic_Reload', N'33', N'0', N'1', N'500', N'35', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'800', N'8', N'3', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'5', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'8001', N'0', N'3002', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400071', N'0', N'0', N'0', N'0', N'HG_beretta92f', N'750');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101112', N'HG_Beretta93R', N'25', N'B93R ', N'The B93R handgun was designed to fire a 9mm parabellum round from a 20 round clip. It is a relatively effective weapon against close ranged targets.', N'0 0 0', N'muzzle_hg', N'pistol', N'5.45', N'Sounds/NewWeapons/Handgun/Beretta93R', N'Sounds/Weapons/HG/HG_Generic_Reload', N'30', N'0', N'1', N'500', N'35', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'800', N'8', N'5', N'0', N'asr_grenade', N'101', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'15', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'8001', N'0', N'3002', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400071', N'0', N'0', N'0', N'0', N'HG_Beretta93R', N'750');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101115', N'HG_Jericho', N'25', N'Jericho 9mm', N'The Jericho was designed as a universal self-loading pistol which fires a 9mm Parabellum round and is highly effective at close range.', N'0 0 0', N'muzzle_hg', N'pistol', N'5.45', N'Sounds/NewWeapons/Handgun/H&K_USP', N'Sounds/Weapons/HG/HG_Generic_Reload', N'35', N'0', N'1', N'500', N'35', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'800', N'9', N'3', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'17', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'8001', N'0', N'3002', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400071', N'0', N'0', N'0', N'0', N'HG_Jericho', N'750');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101120', N'HG_SigP226', N'25', N'Sig Sauer P226', N'The SIG P226 is a full-sized, service-type pistol made by SIG Sauer. It been designed to fire a 9 x 19 mm round that makes it an effective weapon against close ranged targets.', N'0 0 0', N'muzzle_hg', N'pistol', N'9mm', N'Sounds/NewWeapons/Handgun/Beretta92', N'Sounds/Weapons/HG/HG_Generic_Reload', N'35', N'0', N'1', N'500', N'35', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'800', N'8', N'3', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'26', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'8001', N'0', N'3002', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400034', N'0', N'0', N'0', N'0', N'HG_SIGP226', N'750');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101158', N'SHG_Mossberg590_01', N'22', N'MOSSBERG 590', N'The Mossberg 590 is a 12 gauge shotgun which has a 8 round storage tube. It is known to do an extremely high amount of damage at close range.', N'0 0 0', N'muzzle_sh1', N'assault', N'buckshot', N'Sounds/NewWeapons/Shotgun/Mossberg', N'Sounds/Weapons/New Reloads/Mossberg_reload', N'24', N'0', N'1', N'300', N'12', N'0', N'0', N'0', N'0', N'1', N'8', N'1', N'60', N'20', N'15', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'5001', N'3001', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400082', N'0', N'0', N'400136', N'0', N'0', N'0', N'0', N'SHG_Mossberg590_01', N'2500');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101169', N'ASR_Mossada', N'20', N'MASADA', N'The Masada was designed for close combat operations and is a gas operated assault rifle that fires a chambered for 5.56x45mm NATO round. It is highly effective at close ranges.', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'Sounds/NewWeapons/Assault/AEK971', N'Sounds/Weapons/New Reloads/G36-Reload', N'28', N'0', N'1', N'500', N'300', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'700', N'9', N'5', N'0', N'asr_grenade', N'101', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'43', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'1001', N'4001', N'0', N'0', N'0', N'0', N'0', N'400119', N'0', N'0', N'400016', N'0', N'0', N'0', N'0', N'ASR_Mossada', N'4000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101172', N'ASR_SIG_SG556', N'20', N'SIG SAUER 556', N'The 556 is a gas operated, rotating bolt assault rifle that fires a 5.56 x 45 mm round and was designed for special operations. It is very effective out to medium ranges.', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'Sounds/NewWeapons/Assault/SIG_551', N'Sounds/Weapons/New Reloads/ASR_SIG_SG556_Reload', N'27', N'0', N'1', N'500', N'300', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'650', N'6', N'6', N'0', N'asr_grenade', N'101', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'15', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'1001', N'4001', N'0', N'0', N'0', N'0', N'0', N'400025', N'0', N'0', N'400016', N'0', N'0', N'0', N'0', N'ASR_SIG_SG556', N'4000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101173', N'ASR_tar21', N'20', N'IMI TAR-21', N'The TAR-21 is an unique gas operated, rotating bolt, bullpup design assault rifle that fires a 5.56 x 45 mm round. It is extremely effective at close and medium range.', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'Sounds/NewWeapons/Assault/HK_G11', N'Sounds/Weapons/New Reloads/G36-Reload', N'33', N'0', N'1', N'500', N'300', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'700', N'6', N'7', N'0', N'asr_grenade', N'101', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'39', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'0', N'0', N'4001', N'0', N'0', N'0', N'0', N'0', N'400059', N'0', N'0', N'400016', N'0', N'0', N'0', N'0', N'ASR_tar21', N'3300');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101180', N'HG_Desert_Eagle', N'25', N'Desert Eagle', N'The Desert Eagle handgun was designed to fire a .50 caliber round and deals a substancial amount of damage to targets at close ranges.', N'0 0 0', N'muzzle_hg', N'pistol', N'5.45', N'Sounds/NewWeapons/Handgun/DesertEagle', N'Sounds/Weapons/HG/HG_Generic_Reload', N'50', N'0', N'1', N'500', N'40', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'800', N'12', N'22', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'45', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'8001', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400086', N'0', N'0', N'0', N'0', N'HG_Desert_Eagle', N'1750');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101183', N'SHG_Keltech', N'22', N'KT DECIDER', N'The KT Decider is a bull-pup shotgun design that has two magazine feed tubes, each side capable of holding 7 shells, for a total capacity of 14+1.', N'0 0 0', N'muzzle_sh1', N'assault', N'buckshot', N'Sounds/NewWeapons/Shotgun/SHG_Keltech', N'Sounds/Weapons/New Reloads/SHG_Keltech_reload', N'25', N'0', N'1', N'300', N'15', N'0', N'0', N'0', N'0', N'1', N'8', N'1', N'50', N'20', N'15', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'35', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'5001', N'3001', N'1001', N'0', N'0', N'0', N'0', N'0', N'0', N'400036', N'0', N'0', N'400136', N'0', N'0', N'0', N'0', N'SHG_Keltech', N'4000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101193', N'ASR_Fn_scar', N'20', N'FN SCAR CQC', N'The SCAR 16 CQC is an advanced assault rifle that fires standard 5.56 x 45 mm rounds effectively out to medium ranges.', N'0 0 0', N'muzzle_asr', N'assault', N'5.56', N'Sounds/NewWeapons/Assault/FN_SCAR', N'Sounds/Weapons/New Reloads/M16-Reload', N'30', N'0', N'1', N'500', N'300', N'0', N'0', N'0', N'0', N'1', N'2', N'1', N'625', N'7', N'3', N'0', N'asr_grenade', N'111', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'30', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'1001', N'4001', N'0', N'0', N'0', N'0', N'0', N'400006', N'0', N'0', N'400016', N'0', N'0', N'0', N'0', N'ASR_Scar', N'3500');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101197', N'MG_G36MG', N'23', N'RA H23', N'The RA LMG36 was designed to fire a 7.62 mm standard round. This machine gun delivers a reasonable amount of accurate fire onto targets at medium ranges.', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'Sounds/NewWeapons/Assault/HK_G36_CMag', N'Sounds/Weapons/New Reloads/MG_G36MG_Reload', N'31', N'0', N'1', N'600', N'300', N'0', N'0', N'0', N'0', N'1', N'4', N'2', N'650', N'15', N'5', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'49', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'5001', N'3001', N'1001', N'0', N'0', N'0', N'0', N'0', N'0', N'400042', N'0', N'0', N'400017', N'0', N'0', N'0', N'0', N'MG_G36MG', N'6000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101200', N'SHG_Usas12', N'22', N'AA-12', N'The AA-12 gauge automatic shotgun was designed to fire 12 gauge rounds from a 20 round storage system. It causes a reasonable amount of damage per round.', N'0 0 0', N'muzzle_sh1', N'assault', N'buckshot', N'Sounds/NewWeapons/Shotgun/Shotgun', N'Sounds/Weapons/SHOTGUN/SHG_Generic_Reload', N'22', N'0', N'1', N'300', N'10', N'0', N'0', N'0', N'0', N'1', N'3', N'1', N'150', N'20', N'25', N'0', N'asr_grenade', N'101', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'45', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'5001', N'3001', N'1001', N'0', N'0', N'0', N'0', N'0', N'0', N'400121', N'0', N'0', N'400050', N'0', N'0', N'0', N'0', N'SHG_Usas12', N'5000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101201', N'SMG_SR-1_Veresk', N'26', N'VERESK SR-2', N'The MTAC Flasher submachine gun was designed to fire a standard 9 mm round at a high rate of fire with reasonable accuracy at close range targets.', N'0 0 0', N'muzzle_asr', N'assault', N'9mm', N'Sounds/NewWeapons/Assault/Kalashnikov_AK74', N'Sounds/Weapons/New Reloads/M7-Reload', N'23', N'0', N'1', N'500', N'35', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'800', N'11', N'5', N'0', N'asr_grenade', N'101', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'27', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'0', N'4006', N'0', N'0', N'0', N'0', N'0', N'400083', N'0', N'0', N'400084', N'0', N'0', N'0', N'0', N'SMG_SR-1_Veresk', N'1600');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101210', N'ASR_Fn_scar_NightCamo', N'20', N'FN SCAR NIGHT STALKER', N'The Scar 17 CQC Nightstalker is a assault rifle that fires standard 5.56 x 45 mm rounds effectively out to medium ranges.', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'Sounds/NewWeapons/Assault/FN_SCAR', N'Sounds/Weapons/New Reloads/M16-Reload', N'32', N'0', N'1', N'500', N'300', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'625', N'7', N'3', N'0', N'asr_grenade', N'111', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'58', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'1001', N'4001', N'0', N'0', N'0', N'0', N'0', N'400006', N'0', N'0', N'400016', N'0', N'0', N'0', N'0', N'asr_scar', N'3500');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101217', N'SNP_AW_Des', N'21', N'MAUSER SRG DESERT', N'The Mauser SP66 Desert sniper rifle fires a .308W round.', N'0 0 0', N'muzzle_asr', N'assault', N'Sniper5.45', N'Sounds/NewWeapons/Sniper/SNP_AW', N'Sounds/Weapons/New Reloads/SNP_AW_reload', N'110', N'0', N'1', N'800', N'600', N'0', N'0', N'0', N'0', N'1', N'3', N'1', N'60', N'3', N'3', N'0', N'asr_grenade', N'100', N'30', N'sv98', N'90', N'0', N'0', N'0', N'0', N'1', N'28', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'6001', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400008', N'0', N'0', N'400070', N'0', N'0', N'0', N'0', N'SNP_AW', N'7000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101224', N'HG_Jericho', N'25', N'STI Eagle Elite .45 ACP', N'The STI Eagle Elite handgun is an upgraded version of the STI Eagle and is designed to fire a .45 ACP round effectively at close ranges.', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'Sounds/NewWeapons/Handgun/DesertEagle', N'Sounds/Weapons/HG/HG_Generic_Reload', N'36', N'0', N'1', N'500', N'45', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'800', N'9', N'9', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'HG_Jericho', N'1500');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101232', N'SNP_AW_Elite', N'21', N'MAUSER SRG ELITE', N'The Mauser SRG Elitet sniper rifle is a Veterans special edition of the standard SRG. It fires a .300 magnum round very effectively at long ranges.', N'0 0 0', N'muzzle_asr', N'assault', N'Sniper5.45', N'Sounds/NewWeapons/Sniper/0.308caliber', N'Sounds/Weapons/SNP/SNP_Generic_Reload', N'300', N'0', N'2', N'600', N'800', N'0', N'0', N'0', N'0', N'5', N'3.5', N'1.45', N'65', N'0', N'0.1', N'0', N'asr_grenade', N'100', N'30', N'aw50', N'50', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'SNP_AW', N'5000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101245', N'SHG_Mossberg590_01_Elite', N'22', N'MOSSBERG 590 ELITE', N'The Mossberg 590 Elite shotgun is a.12 gauge shotgun like the standard 590 but it causes more damage at an extended range.', N'0 0 0', N'muzzle_sh1', N'assault', N'buckshot', N'Sounds/NewWeapons/Shotgun/Mossberg', N'Sounds/Weapons/New Reloads/Mossberg_reload', N'24', N'0', N'1', N'300', N'12', N'0', N'0', N'0', N'0', N'1', N'8', N'1', N'60', N'20', N'15', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'5001', N'3001', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400082', N'0', N'0', N'400136', N'0', N'0', N'0', N'0', N'SHG_Mossberg590_01', N'2500');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101246', N'SMG_PS90', N'26', N'FN P90 S', N'The FN P90S submachine gun is a highly effective weapon that carries a unique 50 round magazine. It fires a standard 5.56 mm round.', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'Sounds/NewWeapons/SMG/FN_P90', N'Sounds/Weapons/New Reloads/G36-Reload', N'26', N'0', N'1', N'500', N'40', N'0', N'0', N'0', N'0', N'1', N'2', N'2', N'900', N'11', N'8', N'0', N'asr_grenade', N'101', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'58', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400054', N'0', N'0', N'400046', N'0', N'0', N'0', N'0', N'smg_p90', N'3000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101247', N'SNP_Blaser_LRS2_01', N'21', N'BLASER R93', N'The Blaser is a bolt action sniper rifle, designed to fire a .308W round from a 5 round clip. It is a highly accurate weapon at longer ranges.', N'0 0 0', N'muzzle_asr', N'assault', N'Sniper5.45', N'Sounds/NewWeapons/Sniper/SNP_Blaser', N'Sounds/Weapons/New Reloads/SNP_Blaser_reload', N'125', N'0', N'1', N'800', N'800', N'0', N'0', N'0', N'0', N'1', N'3', N'2', N'45', N'3', N'2', N'0', N'asr_grenade', N'100', N'30', N'psg1', N'70', N'0', N'0', N'0', N'0', N'1', N'38', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'6001', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400008', N'0', N'0', N'400070', N'0', N'0', N'0', N'0', N'snp_blaser_lrs2_01', N'6000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101256', N'Consumables_Med_01', N'28', N'Antibiotics', N'Over the counter antibiotics that aid in curing colds and other minor ailments.', N'0 0 0', N'muzzle_asr', N'mine', N'5.45', N'', N'', N'25', N'1', N'10', N'300', N'50', N'0', N'0', N'0', N'1', N'1', N'4', N'2', N'10', N'20', N'20', N'0', N'asr_grenade', N'101', N'30', N'default', N'0', N'0', N'0', N'0', N'5', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'100');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101261', N'Item_bandage_01', N'28', N'Bandages ', N'Common bandages which heal minor wounds and stops excessive bleeding.', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'', N'', N'10', N'1', N'10', N'300', N'50', N'0', N'0', N'0', N'1', N'1', N'4', N'2', N'20', N'20', N'20', N'0', N'asr_grenade', N'101', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1000', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'200');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101262', N'Item_bandage_02', N'28', N'Bandages DX', N'Medical bandages which heal moderate wounds and stops excessive bleeding.', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'', N'', N'45', N'1', N'10', N'300', N'50', N'0', N'0', N'0', N'1', N'5', N'4', N'2', N'30', N'20', N'20', N'0', N'asr_grenade', N'101', N'30', N'default', N'0', N'0', N'0', N'0', N'40', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'100');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101267', N'MEL_Knife_01', N'29', N'Tactical Knife', N'A standard issue knife with an 8" fixed blade, this knife was first designed in 1985 for the United States Army and was later adapted by several other countries as well as mercenary groups throughout the world due to its sturdy design and stopping power.', N'0 0 0', N'', N'melee', N'melee_sharp', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'40', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'120', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'20', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'MEL_Knife', N'600');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101270', N'ASR_Famas_french', N'20', N'FAMAS F1 French', N'The Famas F1 assault rifle fires a standard 5.56mm round using a delayed blowback system. It delivers highly effective fire out of medium ranges.', N'0 0 0', N'default', N'assault', N'5.45', N'Sounds/NewWeapons/Assault/Famas_F1', N'Sounds/Weapons/ASR/ASR_Generic_Reload', N'150', N'1', N'10', N'300', N'80', N'0', N'0', N'0', N'0', N'30', N'2.5', N'0.43', N'900', N'1', N'1', N'0', N'asr_grenade', N'011', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'1001', N'4001', N'30100', N'20100', N'10100', N'0', N'400014', N'400041', N'0', N'0', N'400016', N'0', N'0', N'0', N'0', N'ASR_Famas', N'1000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101277', N'AKBRAZILL', N'25', N'AK Brazill', N'Commonly used in construction', N'0 0 0', N'', N'pistol', N'5.45', N'Sounds/NewWeapons/Handgun/Nailgun', N'Sounds/Weapons/New Reloads/Nailgun-reload', N'10', N'0', N'10', N'300', N'5', N'0', N'0', N'0', N'0', N'1', N'3', N'1', N'120', N'15', N'5', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400157', N'0', N'0', N'0', N'0', N'Mel_NailGun', N'0');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101278', N'MEL_BaseballBat_01', N'29', N'Bat', N'Handy for those times when you need to beat the crap outta something or somebody.', N'0 0 0', N'', N'melee', N'melee', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'25', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'90', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'MEL_Knife', N'750');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101283', N'consumables_bag_chips_01', N'30', N'Bag of Chips', N'A bag of potato chips.  Relieves minor hunger. BBQ Flavor!', N'', N'', N'', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'-5', N'10', N'0', N'', N'', N'30', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'350');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101284', N'Consumables_Bag_MRE_01', N'30', N'Bag MRE', N'Meals Ready-to-Eat.  Extremely convenient! An MRE can be eaten cold, right out of the pouch but they always taste better hot. Each serving size substantially relieves hunger and thirst', N'', N'', N'', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'25', N'0', N'0', N'60', N'100', N'0', N'', N'', N'30', N'', N'0', N'0', N'0', N'0', N'5', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'400');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101285', N'Consumables_Bag_Oat_01', N'30', N'Instant Oatmeal', N'A heart warming way to start your day. Classic Maple and Brown Sugar. Relieves a moderate amount of hunger.', N'', N'', N'', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'5', N'15', N'0', N'', N'', N'30', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'220');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101286', N'Consumables_Coconutwater_01', N'33', N'Coconut Water', N'A bottle of delicious coconut water that when consumed quenches moderate thirst.', N'', N'', N'', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'5', N'5', N'0', N'0', N'55', N'0', N'0', N'', N'', N'30', N'', N'0', N'0', N'0', N'0', N'10', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'350');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101288', N'Consumables_Bar_Chocolate_01', N'30', N'Chocolate Bar', N'A milk chocolate candy bar. The finest in gourmet chocolate. Each serving size provides minor sustenance.', N'', N'', N'', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'10', N'0', N'', N'', N'30', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'220');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101289', N'Consumables_Bar_Granola_01', N'30', N'Granola Bar', N'An organic granola bar.  Each serving size helps relieve moderate hunger.', N'', N'', N'', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'25', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'10', N'0', N'', N'', N'30', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'220');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101290', N'Consumables_Can_Pasta_01', N'30', N'Can of Pasta', N'A can of delicious Pasta-O''s. Fortified with iron and vitamins. Low sugar and low fat. Suitable for Vegetarians. \r\nGluten Free', N'', N'', N'', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'35', N'0', N'', N'', N'30', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'350');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101291', N'Consumables_Can_Soup_01', N'30', N'Can of Soup', N'A can of creamy tomato soup. No need to add water. Relieves moderate hunger.', N'', N'', N'', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'10', N'0', N'0', N'10', N'35', N'0', N'', N'', N'30', N'', N'0', N'0', N'0', N'0', N'10', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'350');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101292', N'Consumables_Can_Spam_01', N'30', N'Can of Ham', N'Mechanically separated Spiced Ham in a 5-ounce cans. May contain pig parts. Relieves moderate hunger.', N'', N'', N'', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'5', N'0', N'0', N'0', N'35', N'0', N'', N'', N'30', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1000', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'350');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101293', N'Consumables_Can_Tuna_01', N'30', N'Can of Tuna', N'Chunk lite savory tuna in water.  Firm, flaky, fresh tasting that you can really tear into. Each serving relieves moderate hunger.', N'', N'', N'', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'5', N'0', N'0', N'10', N'35', N'0', N'', N'', N'30', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'350');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101294', N'Consumables_Energydrink_01', N'33', N'Energy drink', N'A caffeine jump-start that when consumed quenches moderate thirst.', N'', N'', N'', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'20', N'0', N'0', N'1', N'5', N'10', N'0', N'0', N'20', N'-5', N'0', N'', N'', N'30', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'3500', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'220');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101295', N'Consumables_gatorade_01', N'33', N'Electro-AID', N'A flavored drink with electrolytes that when consumed quenches moderate thirst.', N'', N'', N'', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'50', N'0', N'0', N'1', N'5', N'5', N'0', N'0', N'60', N'5', N'0', N'', N'', N'30', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'10000', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'350');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101296', N'Consumables_soda_01', N'33', N'Can of soda', N'A carbonated beverage that when consumed quenches moderate thirst.', N'', N'', N'', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'6', N'0', N'0', N'0', N'15', N'5', N'0', N'', N'', N'30', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'350');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101297', N'Consumables_Tetrapack_Juice_01', N'33', N'Juice', N'A healthy drink that when consumed quenches moderate thirst.', N'', N'', N'', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'5', N'0', N'0', N'30', N'10', N'0', N'', N'', N'30', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'350');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101298', N'Consumables_water_L_01', N'33', N'Water 1L', N'A large bottle of water that when consumed quenches moderate thirst.', N'', N'', N'', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'50', N'0', N'0', N'', N'', N'30', N'', N'0', N'0', N'0', N'0', N'5', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'1000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101299', N'Consumables_water_S_01', N'33', N'Water 375ml', N'A small bottle of water that when consumed quenches moderate thirst.', N'', N'', N'', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'25', N'0', N'0', N'', N'', N'30', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'500', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'375');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101300', N'Consumables_PainKiller_01', N'28', N'Pain killers', N'Over the counter pain killers that counter-act the effects of pain on the body.', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'', N'', N'15', N'1', N'10', N'300', N'50', N'0', N'0', N'0', N'1', N'1', N'4', N'2', N'10', N'20', N'20', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'2000', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'100');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101301', N'Consumables_ZombieRepellent_01', N'28', N'Zombie Repellent', N'A spray on solution that when applied to the skin, it enters the blood stream and masks the smell of fresh blood.', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'', N'', N'20', N'1', N'10', N'300', N'50', N'0', N'0', N'0', N'1', N'1', N'4', N'2', N'10', N'20', N'20', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'10', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'500');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101302', N'Item_Syringe_01_Vaccine', N'28', N'C01-Vaccine', N'An early prototype vaccine that when injected into your bloodstream fights off the infection and lowers blood toxicity levels. Its effectiveness is low however in comparison to later developed vaccines.', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'', N'', N'20', N'1', N'10', N'300', N'50', N'0', N'0', N'0', N'1', N'1', N'4', N'2', N'10', N'20', N'20', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'2000', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'300');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101303', N'Item_Syringe_01_GC_Vaccine', N'28', N'C04-Vaccine', N'A cure to the infection that was developed only weeks after the initial signs of the infection.   Having had only a few weeks of production before the infected had taken over, supply is very low and demand is extremely high for these valuable vaccines.', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'', N'', N'100', N'1', N'10', N'300', N'50', N'0', N'0', N'0', N'1', N'1', N'4', N'2', N'10', N'20', N'20', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'10', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'300');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101304', N'item_medkit_01', N'28', N'Medkit', N'A standard field medical kit commonly used by Medics to heal wounded soldiers on the battlefield. Very effective', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'', N'', N'100', N'1', N'10', N'300', N'50', N'0', N'0', N'0', N'1', N'1', N'4', N'2', N'10', N'20', N'20', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'15', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'200');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101305', N'Item_MsgBottle', N'28', N'Time Capsule', N'Write a message and leave it anywhere in the world for others to find', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'', N'', N'20', N'1', N'10', N'300', N'50', N'0', N'0', N'0', N'1', N'5', N'4', N'2', N'10', N'20', N'20', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'30', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'450');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101306', N'MEL_Flashlight', N'29', N'Flashlight', N'Flashlight for night time, also blinds other players and doubles as a last resort blunt object to bash in the heads of the infected', N'0 0 0', N'', N'melee', N'melee', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'15', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'90', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'500', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'MEL_Flashlight', N'650');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101307', N'MEL_Hammer', N'29', N'Hammer', N'Go with the funk, it is said. If you cant groove to this then you probably are dead. Stop, Hammer time.', N'0 0 0', N'', N'melee', N'melee', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'35', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'90', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'500', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'MEL_Hammer', N'700');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101308', N'MEL_Hatchet', N'29', N'Hatchet', N'Cold-Steel, drop forged head with differential heat treatment. American Hickory Handle.  Sharp corners will easily pierce the thickest skull and the razor sharp edge will shear through flesh and bone like it was white bread.', N'0 0 0', N'', N'melee', N'melee_sharp', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'50', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'90', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'25', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'MEL_Hatchet', N'600');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101309', N'MEL_Pickaxe', N'29', N'Pickaxe', N'Spiked head, flat chisel. Has a good chance of getting stuck in the skull. Awkward to use in tight quarters.', N'0 0 0', N'', N'melee', N'melee_sharp', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'40', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'120', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'20', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'MEL_Pickaxe', N'1000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101310', N'EXP_M26', N'27', N'Frag Grenade', N'The fragmentation grenade is used to spray shrapnel upon exploding. Pull the pin and dont forget to throw it far.. Fire in the hole!', N'0 0 0', N'', N'grenade', N'grenade', N'', N'', N'50', N'0', N'10', N'20', N'50', N'10', N'4', N'0', N'1', N'1', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'EXP_M26', N'180');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101313', N'MEL_BaseballBat_Spikes_01', N'29', N'Spiked Bat', N'Just your traditional baseball bat. With spikes. Razor sharp, sure to leave a mark.', N'0 0 0', N'', N'melee', N'melee', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'45', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'90', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1000', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'MEL_BaseballBat_Spikes_01', N'900');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101314', N'MEL_BaseballBat_02', N'29', N'Metal Bat', N'Aluminum, will never rust. Heads bounce off faster. Not suitable for major league destruction.', N'0 0 0', N'', N'melee', N'melee', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'40', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'90', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'15', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'MEL_BaseballBat_02', N'800');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101315', N'Item_Binocular', N'28', N'Binoculars', N'A great way to checkout if an area is safe before entering, keep an eye out for infected and bandits', N'0 0 0', N'', N'assault', N'5.45', N'', N'', N'0', N'1', N'1', N'1', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'60', N'0', N'0', N'0', N'asr_grenade', N'100', N'30', N'binoculars', N'80', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'Item_Binocular', N'200');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101316', N'Item_Barricade_BarbWire', N'28', N'BarbWire', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'300', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'15', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'300');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101317', N'Item_Barricade_Wood', N'28', N'WoodShield', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'300', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'20', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'300');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101318', N'Item_Barricade_RiotShield', N'28', N'RiotShield', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'300', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'25', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'300');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101319', N'Item_RangeFinder', N'28', N'Range Finder', N'Need to know how far away something is? The range finder can be your best friend when sneaking around searching for survival items', N'0 0 0', N'', N'assault', N'5.45', N'', N'', N'0', N'1', N'1', N'1', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'60', N'0', N'0', N'0', N'asr_grenade', N'100', N'30', N'rangefinder', N'80', N'0', N'0', N'0', N'50', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'Item_RangeFinder', N'500');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101320', N'HG_FlareGun', N'25', N'Flare Gun', N'', N'0 0 0', N'muzzle_hg', N'pistol', N'Flare', N'Sounds/NewWeapons/SMG/HK_MP5SD', N'Sounds/Weapons/HG/HG_Generic_Reload', N'5', N'0', N'10', N'50', N'50', N'0', N'180', N'0', N'0', N'1', N'2', N'2', N'60', N'15', N'5', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400152', N'0', N'0', N'0', N'0', N'HG_FlareGun', N'1000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101321', N'SHG_DoubleBarrel', N'22', N'Double Barrel', N'Also known as Doubles or Broomsticks, this powerful shotgun allows two shots to be fired in quick succession before reloading', N'0 0 0', N'muzzle_sh1', N'assault', N'buckshot', N'Sounds/NewWeapons/Shotgun/Shotgun', N'Sounds/Weapons/New Reloads/SH_Double_barrel_reload', N'20', N'0', N'1', N'300', N'10', N'0', N'0', N'0', N'0', N'1', N'2', N'1', N'120', N'20', N'25', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400141', N'0', N'0', N'0', N'0', N'SHG_DoubleBarrel', N'3000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101322', N'SNP_CrossBow', N'20', N'Compound Crossbow', N'String drawn compound hunting crossbow for silent kills. Uses bow arrows for ammo', N'0 0 0', N'', N'assault', N'Sniper5.45', N'Sounds/NewWeapons/Special/Crossbow', N'Sounds/Weapons/New Reloads/Crossbow-Reload', N'20', N'1', N'1', N'600', N'50', N'0', N'0', N'0', N'0', N'1', N'2', N'2', N'60', N'2', N'2', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'5001', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400140', N'0', N'0', N'0', N'0', N'SNP_CrossBow', N'3500');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101323', N'Item_Airhorn', N'28', N'Air horn', N'Do your friends keep getting attacked?  Be the hero and blow your air horn to make the infected come after you instead.', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'0', N'1', N'1', N'1', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'60', N'0', N'0', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'Item_Airhorn', N'100');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101330', N'HG_Ruger', N'25', N'Ruger', N'Reliable and relatively common, the lower powered Kruger .22 is a household staple, allowing for low cost practice with firm control', N'0 0 0', N'muzzle_hg', N'pistol', N'5.45', N'Sounds/NewWeapons/Handgun/Ruger22', N'Sounds/Weapons/HG/HG_Generic_Reload', N'20', N'0', N'1', N'500', N'35', N'0', N'0', N'0', N'0', N'1', N'2', N'2', N'800', N'9', N'3', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'8001', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400144', N'0', N'0', N'0', N'0', N'HG_Ruger', N'600');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101331', N'HG_Colt_Anaconda', N'25', N'Colt Anaconda', N'Introduced in 1990, the Colt Anaconda is a large double action .44 caliber revolver featuring a six round cylinder, designed and produced by the Colt''s Manufacturing Company', N'0 0 0', N'muzzle_hg', N'pistol', N'5.45', N'Sounds/NewWeapons/Handgun/DesertEagle', N'Sounds/Weapons/HG/HG_Generic_Reload', N'45', N'0', N'1', N'500', N'40', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'800', N'10', N'20', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'20', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400145', N'0', N'0', N'0', N'0', N'HG_Colt_Anaconda', N'1750');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101332', N'ASR_Ruger_Rifle', N'20', N'Kruger .22 Rifle', N'Semi-automatic .22 Long Rifle with removable rotary magazine', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'Sounds/NewWeapons/Assault/Ruger22', N'Sounds/Weapons/New Reloads/ASR_Ruger_rifle_reload', N'25', N'0', N'1', N'500', N'300', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'650', N'9', N'20', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'0', N'7001', N'0', N'0', N'0', N'0', N'0', N'400149', N'0', N'0', N'400148', N'0', N'0', N'0', N'0', N'ASR_Ruger_Rifle', N'2500');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101334', N'ASR_Mini14', N'20', N'Kruger Mini-14', N'The Kruger''s short size makes it ideal for all situations where ease of maneuverability is a priority, whether it be on the ranch or the deep woods.', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'Sounds/NewWeapons/Assault/Ruger22', N'Sounds/Weapons/New Reloads/ASR_Mini14_reload', N'25', N'0', N'1', N'500', N'300', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'650', N'9', N'20', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'3001', N'0', N'7020', N'0', N'0', N'0', N'0', N'0', N'400151', N'0', N'0', N'400150', N'0', N'0', N'0', N'0', N'ASR_Mini14', N'3100');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101335', N'MEL_CandyCane_01', N'29', N'Kandy Kane', N'For yuletide slaughtering in style!', N'0 0 0', N'', N'melee', N'melee', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'15', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'120', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'10', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'MEL_CandyCane_01', N'800');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101336', N'mel_katana_01', N'29', N'Katana', N'Ancient deadly sharp Samurai weapon, hand made to cleave your foe cleanly in two. Or three...or four....', N'0 0 0', N'', N'melee', N'melee_sharp', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'50', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'120', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'15', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'mel_katana_01', N'1200');
GO
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101337', N'mel_katana_01_special', N'29', N'Jokoto Katana', N'Among the most ancient blades, the Jokoto Katana''s were manufactured for only the most elite and honorable of warriors. Carrying such a blade imparts great responsibility.', N'0 0 0', N'', N'melee', N'melee_sharp', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'50', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'90', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'25', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'mel_katana_01', N'1200');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101338', N'MEL_katana_02_special', N'29', N'Wakizashi', N'Shorter than its counterpart, the wakizashi is designed for closer combat and even ritual suicide.', N'0 0 0', N'', N'melee', N'melee_sharp', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'40', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'90', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'30', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'mel_katana_01', N'600');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101339', N'MEL_Machete_01', N'29', N'Machete', N'A rough but sharp blade used for clearing brush', N'0 0 0', N'', N'melee', N'melee_sharp', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'50', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'90', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'30', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'mel_machete_01', N'800');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101340', N'Consumables_MiniSaints_01', N'30', N'MiniSaints', N'Creamy, moist and delectable, MiniSaints are a delicious treat the whole family can enjoy! Best consumed while watching family movies, possibly involving snow and dogs.', N'', N'', N'', N'', N'', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'5', N'0', N'0', N'0', N'5', N'15', N'0', N'', N'', N'30', N'', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1000', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'400');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101341', N'ASR_Mosin_Rifle', N'20', N'Mosin', N'A Russian bolt action rifle with an internally fed magazine, this reliable rifle has been in popular use through countless wars since 1891', N'0 0 0', N'muzzle_asr', N'assault', N'Sniper5.45', N'Sounds/NewWeapons/Assault/Mosin', N'Sounds/Weapons/New Reloads/Mosin-reload', N'50', N'0', N'1', N'500', N'500', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'40', N'2', N'8', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'2001', N'5001', N'0', N'0', N'7030', N'0', N'0', N'0', N'0', N'0', N'400155', N'0', N'0', N'400153', N'0', N'0', N'0', N'0', N'ASR_Mosin_Rifle', N'3000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101342', N'HG_colt_1911', N'25', N'1911', N'Standard US service pistol known for its minimal recoil and light weight', N'0 0 0', N'muzzle_hg', N'pistol', N'Sniper5.45', N'Sounds/NewWeapons/Handgun/9mmGeneric', N'Sounds/Weapons/HG/HG_Generic_Reload', N'35', N'0', N'1', N'500', N'40', N'0', N'0', N'0', N'0', N'1', N'2', N'0', N'800', N'4', N'2', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'500', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'8001', N'0', N'3002', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400154', N'0', N'0', N'0', N'0', N'HG_colt_1911', N'650');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101343', N'MEL_BrassKnuckles', N'29', N'Brass Knuckles', N'A tried and true classic. Pummel your enemies into submission!', N'0 0 0', N'', N'melee', N'melee', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'30', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'90', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'25', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'mel_machete', N'400');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101344', N'MEL_Canoe_paddle', N'29', N'Canoe Paddle', N'Wooden canoe paddle', N'0 0 0', N'', N'melee', N'melee', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'30', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'90', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'15', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'MEL_Canoe_paddle', N'1000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101345', N'MEL_Cricket_bat', N'29', N'Cricket bat', N'Score some runs and crush some skulls!', N'0 0 0', N'', N'melee', N'melee', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'35', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'90', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'20', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'MEL_Cricket_bat', N'800');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101346', N'MEL_Shovel', N'29', N'Shovel', N'Practical AND deadly', N'0 0 0', N'', N'melee', N'melee', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'30', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'90', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'20', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'MEL_Shovel', N'700');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101347', N'MEL_Chainsaw', N'29', N'Chainsaw', N'Tear through undead and living alike with this fearsome household tool', N'0 0 0', N'', N'melee', N'melee', N'', N'', N'50', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'120', N'0', N'0', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'50', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'MEL_Chainsaw', N'1200');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101348', N'Item_Lockbox_01_Crate', N'28', N'Personal locker', N'Placeable personal storage unit', N'0 0 0', N'muzzle_asr', N'assault', N'5.45', N'', N'', N'20', N'1', N'10', N'300', N'50', N'0', N'0', N'0', N'0', N'1', N'4', N'2', N'10', N'20', N'20', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'Item_Lockbox_01_Crate', N'5000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101352', N'Block_Door_Wood_2M_01', N'28', N'Wooden door block', N'2 meters high', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'250', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'300');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101353', N'Block_Wall_Metal_2M_01', N'28', N'Metal wall block', N'2 meters high', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'500', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'500');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101354', N'Block_Wall_Brick_Tall_01', N'28', N'Tall brick wall block', N'Large brick wall piece', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'250', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'400');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101355', N'Block_Wall_Wood_2M_01', N'28', N'Wooden wall piece, 2M', N'2 meter tall wooden wall piece', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'250', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'350');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101356', N'Block_Wall_Brick_Short_01', N'28', N'Short brick wall piece', N'Short brick wall block', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'500', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'400');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101357', N'Block_Farm_01', N'28', N'Farm Block', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'250', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'400');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101358', N'Block_PowerGen_Indust_01', N'28', N'Industrial power generator', N'Industrial strength power generator', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'300', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'500');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101359', N'Block_PowerGen_01_Small', N'28', N'Small Power Generator', N'Small Power Generator', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'200', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'300');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101360', N'Block_SolarWater_01', N'28', N'Solar Water Purifier', N'Small solar powered water purifier', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'200', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'500');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101361', N'Block_Light_01', N'28', N'Placeable light', N'Moveable, placeable light source', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'200', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'200');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101362', N'Block_Farm_ChipBag_01', N'28', N'Bag of Chips farm block', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'200', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'300');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101363', N'Block_Veg_Apple_01', N'28', N'Apple farm block', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'200', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'350');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101364', N'Block_Veg_Banana_01', N'28', N'Banana farm block', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'300', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'300');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101365', N'Block_Veg_Carrot_01', N'28', N'Carrot farm block', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'300', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'350');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101366', N'Block_Veg_Lettuce', N'28', N'Lettuce farm block', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'300', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'350');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101367', N'Block_Veg_PineApple_01', N'28', N'Pineapple farm block', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'300', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'350');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101368', N'Block_Veg_Potato_01', N'28', N'Potato Farm Block', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'300', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'350');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101369', N'Block_Veg_Tomato_01', N'28', N'Tomato Farm Block', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'300', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'350');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101370', N'Block_Veg_WaterMelon_01', N'28', N'Watermelon Farm Block', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'300', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'350');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101371', N'Block_Door_Wood_2M_01_Crate', N'28', N'Wood Door Crate 2M', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'300', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'350');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101372', N'Block_Farm_01_Crate', N'28', N'Farm Block Crate', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'300', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'300');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101373', N'Block_Light_01_Crate', N'28', N'Light block crate', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'300', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'300');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101374', N'Block_PowerGen_01_Crate', N'28', N'Power Generator Block Crate', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'300', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'300');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101375', N'Block_PowerGen_Indust_01_Crate', N'28', N'Industrial Power Generator block', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'300', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'300');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101376', N'Block_SolarWater_01_Crate', N'28', N'Solar Water Power block crate', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'300', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'300');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101377', N'Block_Wall_Brick_Short_01_Crate', N'28', N'Short Brick Wall crate', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'300', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'300');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101378', N'Block_Wall_Brick_Tall_01_Crate', N'28', N'Tall Brick wall crate', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'300', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'300');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101379', N'Block_Wall_Metal_2M_01_Crate', N'28', N'Tall 2M Metal Wall Block crate', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'300', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'300');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101380', N'Block_Wall_Wood_2M_01_Crate', N'28', N'Wood Wall block crate, 2M', N'', N'0 0 0', N'', N'assault', N'melee', N'', N'', N'300', N'1', N'10', N'60', N'1', N'0', N'0', N'0', N'1', N'5', N'1', N'0', N'60', N'1', N'1', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'300');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101381', N'Mel_ButterflyKnife', N'29', N'Butterfly Knife', N'A folding knife with two handles counter rotating around the blade so that when its closed the blade concealed', N'0 0 0', N'', N'melee', N'melee_sharp', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'40', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'120', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'20', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'Mel_ButterflyKnife', N'400');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101382', N'Mel_Crowbar', N'29', N'Crowbar', N'A multi use household tool, useful for construction or surviving an apocalyptic event', N'0 0 0', N'', N'melee', N'melee', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'30', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'90', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'30', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'Mel_Crowbar', N'600');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101383', N'Mel_FireAxe', N'29', N'Fire Axe', N'Standard issue axe used by firefighters to enter buildings and remove dangerous debris', N'0 0 0', N'', N'melee', N'melee_sharp', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'40', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'90', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'40', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'Mel_FireAxe', N'1200');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101384', N'Mel_FryingPan', N'29', N'Frying Pan', N'Whether frying an egg or flattening a face, the common household frying pan has many uses', N'0 0 0', N'', N'melee', N'melee', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'20', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'90', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'40', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'Mel_FryingPan', N'1100');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101385', N'Mel_GardenShears', N'29', N'Garden Shears', N'Whether shortening hedges or heads these shears are sure to come in handy', N'0 0 0', N'', N'melee', N'melee_sharp', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'40', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'90', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'40', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'Mel_GardenShears', N'1000');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101386', N'Mel_Golf_Club', N'29', N'Golf Club', N'Best used for long drives. FORE!', N'0 0 0', N'', N'melee', N'melee', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'30', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'90', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'40', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'Mel_Golf_Club', N'800');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101388', N'Mel_Pitchfork', N'29', N'Pitchfork', N'Designed to move hay economically, its intimidating design suits it well to sticking some undead', N'0 0 0', N'', N'melee', N'melee_sharp', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'40', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'90', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'40', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'Mel_Pitchfork', N'1400');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101389', N'Mel_PoliceBaton', N'29', N'Police Baton', N'Used by law enforcement as an aid in hand to hand combat, often to subdue unruly or resistant suspects', N'0 0 0', N'', N'melee', N'melee', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'30', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'90', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'50', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'700');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101390', N'Mel_Power_Drill', N'29', N'Power Drill', N'Battery powered and unconventional, the drill is quite effective at dispatching enemies in the grisliest way', N'0 0 0', N'', N'melee', N'melee_sharp', N'', N'', N'40', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'120', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'50', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'900');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101391', N'Mel_Wrench', N'29', N'Wrench', N'Loved by plumbers around the world, this hefty tool can easily collapse the skulls of your foes', N'0 0 0', N'', N'melee', N'melee', N'Sounds/NewWeapons/Melee/Melee_Whoosh', N'', N'20', N'1', N'10', N'300', N'1', N'0', N'0', N'0', N'0', N'1', N'0', N'0', N'90', N'0', N'0', N'0', N'asr_grenade', N'001', N'30', N'default', N'0', N'0', N'0', N'0', N'40', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'', N'1200');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'101392', N'Mel_NailGun', N'25', N'Nail Gun', N'Commonly used in construction', N'0 0 0', N'', N'pistol', N'5.45', N'Sounds/NewWeapons/Handgun/Nailgun', N'Sounds/Weapons/New Reloads/Nailgun-reload', N'10', N'0', N'10', N'300', N'5', N'0', N'0', N'0', N'0', N'1', N'3', N'1', N'120', N'15', N'5', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400157', N'0', N'0', N'0', N'0', N'Mel_NailGun', N'0');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'301319', N'DUCK TAPE', N'25', N'Duck Tape', N'Commonly used in construction', N'0 0 0', N'', N'pistol', N'5.45', N'Sounds/NewWeapons/Handgun/Nailgun', N'Sounds/Weapons/New Reloads/Nailgun-reload', N'10', N'0', N'10', N'300', N'5', N'0', N'0', N'0', N'0', N'1', N'3', N'1', N'120', N'15', N'5', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400157', N'0', N'0', N'0', N'0', N'Mel_NailGun', N'0');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'301328', N'Nails', N'25', N'Nails', N'Commonly used in construction', N'0 0 0', N'', N'pistol', N'5.45', N'Sounds/NewWeapons/Handgun/Nailgun', N'Sounds/Weapons/New Reloads/Nailgun-reload', N'10', N'0', N'10', N'300', N'5', N'0', N'0', N'0', N'0', N'1', N'3', N'1', N'120', N'15', N'5', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400157', N'0', N'0', N'0', N'0', N'Mel_NailGun', N'0');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'301335', N'SC', N'25', N'Scisor', N'Commonly used in construction', N'0 0 0', N'', N'pistol', N'5.45', N'Sounds/NewWeapons/Handgun/Nailgun', N'Sounds/Weapons/New Reloads/Nailgun-reload', N'10', N'0', N'10', N'300', N'5', N'0', N'0', N'0', N'0', N'1', N'3', N'1', N'120', N'15', N'5', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400157', N'0', N'0', N'0', N'0', N'Mel_NailGun', N'0');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'301339', N'Thread', N'25', N'Thread', N'Commonly used in construction', N'0 0 0', N'', N'pistol', N'5.45', N'Sounds/NewWeapons/Handgun/Nailgun', N'Sounds/Weapons/New Reloads/Nailgun-reload', N'10', N'0', N'10', N'300', N'5', N'0', N'0', N'0', N'0', N'1', N'3', N'1', N'120', N'15', N'5', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400157', N'0', N'0', N'0', N'0', N'Mel_NailGun', N'0');
INSERT INTO [dbo].[Items_Weapons] ([ItemID], [FNAME], [Category], [Name], [Description], [MuzzleOffset], [MuzzleParticle], [Animation], [BulletID], [Sound_Shot], [Sound_Reload], [Damage], [isImmediate], [Mass], [Speed], [DamageDecay], [Area], [Delay], [Timeout], [NumClips], [Clipsize], [ReloadTime], [ActiveReloadTick], [RateOfFire], [Spread], [Recoil], [NumGrenades], [GrenadeName], [Firemode], [DetectionRadius], [ScopeType], [ScopeZoom], [Price1], [Price7], [Price30], [PriceP], [IsNew], [LevelRequired], [GPrice1], [GPrice7], [GPrice30], [GPriceP], [ShotsFired], [ShotsHits], [KillsCQ], [KillsDM], [KillsSB], [IsUpgradeable], [IsFPS], [FPSSpec0], [FPSSpec1], [FPSSpec2], [FPSSpec3], [FPSSpec4], [FPSSpec5], [FPSSpec6], [FPSSpec7], [FPSSpec8], [FPSAttach0], [FPSAttach1], [FPSAttach2], [FPSAttach3], [FPSAttach4], [FPSAttach5], [FPSAttach6], [FPSAttach7], [FPSAttach8], [AnimPrefix], [Weight]) VALUES (N'301370', N'ROPE', N'25', N'Rope', N'Commonly used in construction', N'0 0 0', N'', N'pistol', N'5.45', N'Sounds/NewWeapons/Handgun/Nailgun', N'Sounds/Weapons/New Reloads/Nailgun-reload', N'10', N'0', N'10', N'300', N'5', N'0', N'0', N'0', N'0', N'1', N'3', N'1', N'120', N'15', N'5', N'0', N'asr_grenade', N'100', N'30', N'default', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'1', N'1', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'0', N'400157', N'0', N'0', N'0', N'0', N'Mel_NailGun', N'0');
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[Leaderboard]
-- ----------------------------
CREATE TABLE [dbo].[Leaderboard] (
[Pos] int NOT NULL IDENTITY(1,1) ,
[CustomerID] int NOT NULL ,
[gamertag] nvarchar(32) NOT NULL ,
[Rank] int NOT NULL ,
[HonorPoints] int NOT NULL ,
[Wins] int NOT NULL ,
[Losses] int NOT NULL ,
[Kills] int NOT NULL ,
[Deaths] int NOT NULL ,
[ShotsFired] int NOT NULL ,
[ShotsHit] int NOT NULL ,
[TimePlayed] int NOT NULL ,
[HavePremium] int NOT NULL 
)


GO

-- ----------------------------
-- Records of Leaderboard
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[Leaderboard] ON
GO
SET IDENTITY_INSERT [dbo].[Leaderboard] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[Leaderboard1]
-- ----------------------------
CREATE TABLE [dbo].[Leaderboard1] (
[Pos] int NOT NULL IDENTITY(1,1) ,
[CustomerID] int NOT NULL ,
[gamertag] nvarchar(32) NOT NULL ,
[Rank] int NOT NULL ,
[HonorPoints] int NOT NULL ,
[Wins] int NOT NULL ,
[Losses] int NOT NULL ,
[Kills] int NOT NULL ,
[Deaths] int NOT NULL ,
[ShotsFired] int NOT NULL ,
[ShotsHit] int NOT NULL ,
[TimePlayed] int NOT NULL ,
[HavePremium] int NOT NULL 
)


GO

-- ----------------------------
-- Records of Leaderboard1
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[Leaderboard1] ON
GO
SET IDENTITY_INSERT [dbo].[Leaderboard1] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[Leaderboard30]
-- ----------------------------
CREATE TABLE [dbo].[Leaderboard30] (
[Pos] int NOT NULL IDENTITY(1,1) ,
[CustomerID] int NOT NULL ,
[gamertag] nvarchar(32) NOT NULL ,
[Rank] int NOT NULL ,
[HonorPoints] int NOT NULL ,
[Wins] int NOT NULL ,
[Losses] int NOT NULL ,
[Kills] int NOT NULL ,
[Deaths] int NOT NULL ,
[ShotsFired] int NOT NULL ,
[ShotsHit] int NOT NULL ,
[TimePlayed] int NOT NULL ,
[HavePremium] int NOT NULL 
)


GO

-- ----------------------------
-- Records of Leaderboard30
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[Leaderboard30] ON
GO
SET IDENTITY_INSERT [dbo].[Leaderboard30] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[Leaderboard7]
-- ----------------------------
CREATE TABLE [dbo].[Leaderboard7] (
[Pos] int NOT NULL IDENTITY(1,1) ,
[CustomerID] int NOT NULL ,
[gamertag] nvarchar(32) NOT NULL ,
[Rank] int NOT NULL ,
[HonorPoints] int NOT NULL ,
[Wins] int NOT NULL ,
[Losses] int NOT NULL ,
[Kills] int NOT NULL ,
[Deaths] int NOT NULL ,
[ShotsFired] int NOT NULL ,
[ShotsHit] int NOT NULL ,
[TimePlayed] int NOT NULL ,
[HavePremium] int NOT NULL 
)


GO

-- ----------------------------
-- Records of Leaderboard7
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[Leaderboard7] ON
GO
SET IDENTITY_INSERT [dbo].[Leaderboard7] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[LoginLog]
-- ----------------------------
CREATE TABLE [dbo].[LoginLog] (
[id] int NOT NULL IDENTITY(1,1) ,
[CustomerID] int NOT NULL ,
[ip] nvarchar(50) NOT NULL ,
[mac] nvarchar(50) NOT NULL ,
[time] datetime NOT NULL ,
[MiniACRC] nvarchar(50) NULL ,
[WarGuardCRC] nvarchar(50) NULL 
)


GO
DBCC CHECKIDENT(N'[dbo].[LoginLog]', RESEED, 734)
GO

-- ----------------------------
-- Records of LoginLog
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[LoginLog] ON
GO
SET IDENTITY_INSERT [dbo].[LoginLog] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[Logins]
-- ----------------------------
CREATE TABLE [dbo].[Logins] (
[LoginID] int NOT NULL IDENTITY(1,1) ,
[CustomerID] int NOT NULL DEFAULT ((0)) ,
[LoginTime] datetime NOT NULL DEFAULT (((12)/(1))/(1973)) ,
[IP] varchar(16) NOT NULL DEFAULT ('1.1.1.1') ,
[LoginSource] int NOT NULL DEFAULT ((0)) 
)


GO
DBCC CHECKIDENT(N'[dbo].[Logins]', RESEED, 734)
GO

-- ----------------------------
-- Records of Logins
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[Logins] ON
GO
SET IDENTITY_INSERT [dbo].[Logins] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[LoginSessions]
-- ----------------------------
CREATE TABLE [dbo].[LoginSessions] (
[CustomerID] int NOT NULL ,
[SessionKey] varchar(50) NOT NULL ,
[SessionID] int NOT NULL DEFAULT ((0)) ,
[LoginIP] varchar(16) NOT NULL ,
[TimeLogged] datetime NOT NULL ,
[TimeUpdated] datetime NOT NULL ,
[GameSessionID] bigint NOT NULL DEFAULT ((0)) 
)


GO

-- ----------------------------
-- Records of LoginSessions
-- ----------------------------
BEGIN TRANSACTION
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[MacBan]
-- ----------------------------
CREATE TABLE [dbo].[MacBan] (
[Mac] nvarchar(50) NOT NULL ,
[Reason] nvarchar(200) NOT NULL 
)


GO

-- ----------------------------
-- Records of MacBan
-- ----------------------------
BEGIN TRANSACTION
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[MasterServerInfo]
-- ----------------------------
CREATE TABLE [dbo].[MasterServerInfo] (
[ServerID] int NOT NULL DEFAULT ((0)) ,
[LastUpdated] datetime NOT NULL DEFAULT (((1)/(1))/(1970)) ,
[CreateGameKey] int NOT NULL DEFAULT ((0)) ,
[IP] varchar(64) NOT NULL DEFAULT ('0.0.0.0') 
)


GO

-- ----------------------------
-- Records of MasterServerInfo
-- ----------------------------
BEGIN TRANSACTION
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[PremiumReceiveGCLog]
-- ----------------------------
CREATE TABLE [dbo].[PremiumReceiveGCLog] (
[Receive] date NOT NULL 
)


GO

-- ----------------------------
-- Records of PremiumReceiveGCLog
-- ----------------------------
BEGIN TRANSACTION
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[SecurityLog]
-- ----------------------------
CREATE TABLE [dbo].[SecurityLog] (
[ID] int NOT NULL IDENTITY(1,1) ,
[EventID] int NOT NULL ,
[Date] datetime NOT NULL ,
[IP] varchar(64) NOT NULL DEFAULT ('0.0.0.0') ,
[CustomerID] int NOT NULL DEFAULT ((0)) ,
[EventData] varchar(256) NOT NULL DEFAULT '' 
)


GO

-- ----------------------------
-- Records of SecurityLog
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[SecurityLog] ON
GO
SET IDENTITY_INSERT [dbo].[SecurityLog] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[ServerNotesData]
-- ----------------------------
CREATE TABLE [dbo].[ServerNotesData] (
[NoteID] int NOT NULL IDENTITY(1,1) ,
[GameServerId] int NOT NULL ,
[GamePos] varchar(128) NULL ,
[CreateUtcDate] datetime NULL ,
[ExpireUtcDate] datetime NULL ,
[CustomerID] int NULL ,
[CharID] int NULL ,
[TextFrom] nvarchar(128) NULL ,
[TextSubj] nvarchar(2048) NULL 
)


GO
DBCC CHECKIDENT(N'[dbo].[ServerNotesData]', RESEED, 5)
GO

-- ----------------------------
-- Records of ServerNotesData
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[ServerNotesData] ON
GO
SET IDENTITY_INSERT [dbo].[ServerNotesData] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[UsersChars]
-- ----------------------------
CREATE TABLE [dbo].[UsersChars] (
[CharID] int NOT NULL IDENTITY(1,1) ,
[CustomerID] int NOT NULL ,
[Hardcore] int NOT NULL ,
[Gamertag] nvarchar(64) NOT NULL ,
[HeroItemID] int NOT NULL DEFAULT ((20170)) ,
[HeadIdx] int NOT NULL ,
[BodyIdx] int NOT NULL ,
[LegsIdx] int NOT NULL ,
[Alive] int NOT NULL DEFAULT ((3)) ,
[DeathUtcTime] datetime NOT NULL DEFAULT ('1/1/1973') ,
[XP] int NOT NULL DEFAULT ((0)) ,
[TimePlayed] int NOT NULL DEFAULT ((0)) ,
[GameMapId] int NOT NULL DEFAULT ((0)) ,
[GameServerId] bigint NOT NULL DEFAULT ((0)) ,
[GamePos] varchar(128) NOT NULL DEFAULT '' ,
[GameFlags] int NOT NULL DEFAULT ((0)) ,
[Health] float(53) NOT NULL DEFAULT ((100)) ,
[Food] float(53) NOT NULL DEFAULT ((0)) ,
[Water] float(53) NOT NULL DEFAULT ((0)) ,
[Toxic] float(53) NOT NULL DEFAULT ((0)) ,
[Reputation] int NOT NULL DEFAULT ((0)) ,
[BackpackID] int NOT NULL DEFAULT ((20176)) ,
[BackpackSize] int NOT NULL DEFAULT ((12)) ,
[Attachment1] varchar(256) NOT NULL DEFAULT '' ,
[Attachment2] varchar(256) NOT NULL DEFAULT '' ,
[Stat00] int NOT NULL DEFAULT ((0)) ,
[Stat01] int NOT NULL DEFAULT ((0)) ,
[Stat02] int NOT NULL DEFAULT ((0)) ,
[Stat03] int NOT NULL DEFAULT ((0)) ,
[Stat04] int NOT NULL DEFAULT ((0)) ,
[Stat05] int NOT NULL DEFAULT ((0)) ,
[LastUpdateDate] datetime NULL ,
[CreateDate] datetime NULL ,
[ClanID] int NOT NULL DEFAULT ((0)) ,
[ClanRank] int NOT NULL DEFAULT ((99)) ,
[ClanContributedXP] int NOT NULL DEFAULT ((0)) ,
[ClanContributedGP] int NOT NULL DEFAULT ((0)) ,
[SkillID1] int NULL ,
[SkillID2] int NULL ,
[SkillID3] int NULL ,
[SkillID4] int NULL ,
[SkillID5] int NULL ,
[SkillID6] int NULL ,
[SkillID7] int NULL ,
[SkillID8] int NULL ,
[SkillID9] int NULL ,
[SkillID10] int NULL ,
[SkillID11] int NULL ,
[SkillID12] int NULL ,
[SkillID13] int NULL ,
[SkillID14] int NULL ,
[SkillID15] int NULL ,
[SkillID16] int NULL ,
[SkillID17] int NULL ,
[SkillID18] int NULL ,
[SkillID19] int NULL ,
[SkillID20] int NULL ,
[SkillID21] int NULL ,
[SkillID22] int NULL ,
[SkillID23] int NULL ,
[SkillID24] int NULL ,
[SkillID25] int NULL ,
[SkillID26] int NULL ,
[SkillID27] int NULL ,
[SkillID28] int NULL ,
[SkillID29] int NULL ,
[SkillID30] int NULL ,
[SkillID31] int NULL ,
[SkillID32] int NULL ,
[SkillID33] int NULL ,
[SkillID0] int NULL ,
[GroupID] int NULL DEFAULT ((0)) ,
[bleeding] int NULL ,
[legfall] int NULL ,
[Mission1] int NULL 
)


GO
DBCC CHECKIDENT(N'[dbo].[UsersChars]', RESEED, 26)
GO

-- ----------------------------
-- Records of UsersChars
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[UsersChars] ON
GO
SET IDENTITY_INSERT [dbo].[UsersChars] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[UsersData]
-- ----------------------------
CREATE TABLE [dbo].[UsersData] (
[CustomerID] int NOT NULL ,
[IsDeveloper] int NOT NULL DEFAULT ((0)) ,
[AccountType] int NOT NULL DEFAULT ((99)) ,
[AccountStatus] int NOT NULL DEFAULT ((100)) ,
[GamePoints] int NOT NULL DEFAULT ((0)) ,
[GameDollars] int NOT NULL DEFAULT ((0)) ,
[dateregistered] datetime NOT NULL DEFAULT ('1/1/1973') ,
[lastjoineddate] datetime NOT NULL DEFAULT ('1/1/1973') ,
[lastgamedate] datetime NOT NULL DEFAULT ('1/1/1973') ,
[ClanID] int NOT NULL DEFAULT ((0)) ,
[ClanRank] int NOT NULL DEFAULT ((99)) ,
[GameServerId] bigint NULL ,
[CharsCreated] int NOT NULL DEFAULT ((0)) ,
[TimePlayed] int NOT NULL DEFAULT ((0)) ,
[DateActiveUntil] datetime NOT NULL DEFAULT ('2030-1-1') ,
[BanTime] datetime NULL ,
[BanReason] nvarchar(512) NULL ,
[BanCount] int NOT NULL DEFAULT ((0)) ,
[BanExpireDate] datetime NULL ,
[IsPunisher] int NULL ,
[ReportCount] int NULL ,
[PremiumExpireDate] datetime NULL ,
[IsPremium] int NULL ,
[WarGuardSessionID] int NULL 
)


GO

-- ----------------------------
-- Records of UsersData
-- ----------------------------
BEGIN TRANSACTION
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[UsersInventory]
-- ----------------------------
CREATE TABLE [dbo].[UsersInventory] (
[InventoryID] bigint NOT NULL IDENTITY(1,1) ,
[CustomerID] int NOT NULL ,
[CharID] int NOT NULL DEFAULT ((0)) ,
[BackpackSlot] int NOT NULL DEFAULT ((0)) ,
[ItemID] int NOT NULL ,
[LeasedUntil] datetime NOT NULL ,
[Quantity] int NOT NULL DEFAULT ((1)) ,
[Var1] int NOT NULL DEFAULT ((-1)) ,
[Var2] int NOT NULL DEFAULT ((-1)) ,
[Repair] int NULL 
)


GO
DBCC CHECKIDENT(N'[dbo].[UsersInventory]', RESEED, 423)
GO

-- ----------------------------
-- Records of UsersInventory
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [dbo].[UsersInventory] ON
GO
SET IDENTITY_INSERT [dbo].[UsersInventory] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[UsersPrivate]
-- ----------------------------
CREATE TABLE [dbo].[UsersPrivate] (
[CustomerID] int NOT NULL ,
[IsJoinPrivate] int NULL ,
[PrivateServerName] varchar(MAX) NULL 
)


GO

-- ----------------------------
-- Records of UsersPrivate
-- ----------------------------
BEGIN TRANSACTION
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[VitalStats_V1]
-- ----------------------------
CREATE TABLE [dbo].[VitalStats_V1] (
[Timestamp] datetime NULL ,
[TotalSales] int NULL ,
[TotalUsers] int NULL ,
[DAU] int NULL ,
[CCU] int NULL ,
[Revenues] int NULL 
)


GO

-- ----------------------------
-- Records of VitalStats_V1
-- ----------------------------
BEGIN TRANSACTION
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for [dbo].[WebLoginSessions]
-- ----------------------------
CREATE TABLE [dbo].[WebLoginSessions] (
[CustomerID] int NOT NULL ,
[SessionKey] varchar(50) NOT NULL ,
[SessionID] int NOT NULL DEFAULT ((0)) ,
[LoginIP] varchar(16) NOT NULL ,
[TimeLogged] datetime NOT NULL ,
[TimeUpdated] datetime NOT NULL 
)


GO

-- ----------------------------
-- Records of WebLoginSessions
-- ----------------------------
BEGIN TRANSACTION
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Procedure structure for [dbo].[AddObject_Allright]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[AddObject_Allright]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[AddObject_Allright]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[AddObject_Allright]
-- ----------------------------
CREATE PROCEDURE [dbo].[AddObject_Allright]
AS
BEGIN
INSERT INTO [dbo].[Items_Gear] VALUES (N'20204', N'Super Zombie', N'Super Zombie', N'', N'16', N'-1', N'0', N'0', N'1', N'1', N'1', N'1', N'7', N'30', N'314', N'0', N'1', N'0', N'11', N'77', N'3030', N'3141');
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[ADMIN_BanUser]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[ADMIN_BanUser]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[ADMIN_BanUser]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[ADMIN_BanUser]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [ADMIN_BanUser]
-- ----------------------------

CREATE PROCEDURE [dbo].[ADMIN_BanUser]
	@in_CustomerID int,
  @in_Reason VARCHAR(512)
AS
BEGIN
	update UsersData set AccountStatus=200 WHERE CustomerID=@in_CustomerID
	update UsersData set BanReason=@in_Reason WHERE CustomerID=@in_CustomerID
	-- clear his login session
	update LoginSessions set SessionID=0 where CustomerID=@in_CustomerID
	
	-- Insert to BanLog
	insert into dbo.DBG_BanLog values (@in_CustomerID, GETDATE(), 300, @in_Reason)
	
  select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[ADMIN_BanUser_BUG]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[ADMIN_BanUser_BUG]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[ADMIN_BanUser_BUG]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[ADMIN_BanUser_BUG]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [ADMIN_BanUser]
-- ----------------------------

CREATE PROCEDURE [dbo].[ADMIN_BanUser_BUG]
	@in_CustomerID int,
	@in_BanReason nvarchar(256)
AS
BEGIN
	SET NOCOUNT ON;
	if(LEN(@in_BanReason) < 4) begin
		select 'GIVE PROPER REASON'
		return
	end

	declare @email varchar(128)
	select @email=email from dbo.Accounts where CustomerID=@in_CustomerID

	-- do not ban multiple times
	declare @AccountStatus int = 0
	declare @AccountType int = 0
	declare @BanCount int = 0
	declare @BanReason nvarchar(512)
	select 
		@AccountStatus=AccountStatus, 
		@accountType=AccountType,
		@BanCount=BanCount, 
		@BanReason=BanReason 
	from UsersData where CustomerID=@in_CustomerID
	--if(@AccountStatus = 200 or @AccountStatus = 201) begin
	--	select 0 as ResultCode, 'already banned' as ResultMsg, @email as 'email'
	--	return
	--end

	-- clear his login session
	update dbo.LoginSessions set SessionID=0 where CustomerID=@in_CustomerID
	
	-- set his all alive chars to respawned mode
	update dbo.UsersChars set Alive=2 where CustomerID=@in_CustomerID and Alive=1

	if(@BanReason is null) set @BanReason = @in_BanReason
	else                   set @BanReason = @in_BanReason + ', ' + @BanReason

	-- guest ban, permament
	if(@AccountType	= 3) begin
		set @BanReason = '[GUEST_BAN] ' + @BanReason
		set @BanCount  = 99;
	end
	
	-- ban
	--if(@BanCount > 0) 
	--begin
	--	insert into dbo.DBG_BanLog values (@in_CustomerID, GETDATE(), 2000, @in_BanReason)

	--	update UsersData set 
	--		AccountStatus=200, 
	--		BanReason=@BanReason, 
	--		BanTime=GETDATE(),
	--		BanCount=(BanCount+1),
	--		BanExpireDate='2030-1-1'
	--		where CustomerID=@in_CustomerID

	--	select 0 as ResultCode, 'Permanent BAN' as ResultMsg, @email as 'email', @BanReason as 'BanReason'
	--	return
	--end
	--else
	--begin
		declare @BanTime int = 3

		insert into dbo.DBG_BanLog values (@in_CustomerID, GETDATE(), @BanTime, @in_BanReason)

		update UsersData set 
			AccountStatus=201, 
			BanReason=@BanReason, 
			BanTime=GETDATE(),
			BanExpireDate=DATEADD(DAY, @BanTime, GETDATE())
			where CustomerID=@in_CustomerID

		select 0 as ResultCode, 'temporary ban' as ResultMsg, @email as 'email', @BanReason as 'BanReason'
		return
	--end
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[ADMIN_BanWeaponHackers]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[ADMIN_BanWeaponHackers]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[ADMIN_BanWeaponHackers]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[ADMIN_BanWeaponHackers]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [ADMIN_BanWeaponHackers]
-- ----------------------------

CREATE PROCEDURE [dbo].[ADMIN_BanWeaponHackers]
AS  
BEGIN  
	SET NOCOUNT ON;  
	declare @hacks TABLE 
	(
		CustomerID int,
		data varchar(512)
	)
	
	declare @dt1 date = DATEADD(day, -1, GETDATE())

	-- select all hack attempts to table
	insert into @hacks 
		select CustomerID, data from DBG_SrvLogInfo where 
			ReportTime >= @dt1 and IsProcessed=0 and CheatID=5 and (data like 'id:%')

	-- clear them
	update DBG_SrvLogInfo set IsProcessed=1 where
		ReportTime >= @dt1 and IsProcessed=0 and CheatID=5

	--
	-- main ban loop
	--
	declare @CustomerID int
	declare @HackData varchar(512)
	
	DECLARE t_cursor CURSOR FOR 
		select customerid, data from @hacks 

	OPEN t_cursor
	FETCH NEXT FROM t_cursor into @CustomerID, @HackData
	while @@FETCH_STATUS = 0 
	begin
		declare @AccountStatus int
		
		-- start banning
		select @AccountStatus=AccountStatus from dbo.UsersData where CustomerID=@CustomerID

		if(@AccountStatus = 100)
		begin
			declare @BanReason varchar(512) = ''
			set @BanReason = 'WH[' + 
				convert(varchar(128), MONTH(GETDATE())) + 
				'/' + 
				convert(varchar(128), DAY(GETDATE())) + 
				'] ' + @HackData
			
			print @CustomerID
			print @BanReason
			exec ADMIN_BanUser @CustomerID, @BanReason
		end
		
		FETCH NEXT FROM t_cursor into @CustomerID, @HackData
	end
	close t_cursor
	deallocate t_cursor

END






GO

-- ----------------------------
-- Procedure structure for [dbo].[ADMIN_GetCustomerID]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[ADMIN_GetCustomerID]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[ADMIN_GetCustomerID]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[ADMIN_GetCustomerID]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [ADMIN_UNBanUser]
-- ----------------------------

CREATE PROCEDURE [dbo].[ADMIN_GetCustomerID]
	@in_CustomerID int
AS
BEGIN
	select * from Accounts where email='ped_ht_14@hotmail.com'
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[ADMIN_UNBanUser]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[ADMIN_UNBanUser]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[ADMIN_UNBanUser]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[ADMIN_UNBanUser]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [ADMIN_UNBanUser]
-- ----------------------------

CREATE PROCEDURE [dbo].[ADMIN_UNBanUser]
	@in_CustomerID int
AS
BEGIN
	update UsersData set AccountStatus=100, BanCount=0, BanExpireDate=NULL, BanReason=NULL, BanTime=NULL WHERE CustomerID=@in_CustomerID
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[AutoLootbox]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[AutoLootbox]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[AutoLootbox]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[AutoLootbox]
-- ----------------------------
CREATE PROCEDURE [dbo].[AutoLootbox]
AS
BEGIN
   declare @ItemID1 int
	SELECT ItemID from Items_Weapons

INSERT INTO Items_LootData (
		LootID,
		Chance,
		ItemID,
GDMin,
GDMax
	) VALUES (
		301138,
		'100',
 @ItemID1,
'0',
'0'
	)
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[DB_PurgeUnusedUserData]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[DB_PurgeUnusedUserData]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[DB_PurgeUnusedUserData]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[DB_PurgeUnusedUserData]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [DB_PurgeUnusedUserData]
-- ----------------------------

-- =============================================
-- Author:		Denis Zhulitov
-- Create date: 03/15/2011
-- Description:	deleting unused records from tables if user record was deleted
-- =============================================
CREATE PROCEDURE [dbo].[DB_PurgeUnusedUserData]
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	delete from Profile_Chars
	where not exists (select * from LoginID where LoginID.CustomerID = Profile_Chars.CustomerID)
	select @@RowCount as Deleted, 'Profile_Chars' as FromTable

	delete from ProfileData
	where not exists (select * from LoginID where LoginID.CustomerID = ProfileData.CustomerID)
	select @@RowCount as Deleted, 'ProfileData' as FromTable

	delete from Stats
	where not exists (select * from LoginID where LoginID.CustomerID = Stats.CustomerID)
	select @@RowCount as Deleted, 'Stats' as FromTable

	delete from Logins
	where not exists (select * from LoginID where LoginID.CustomerID = Logins.CustomerID)
	select @@RowCount as Deleted, 'Logins' as FromTable

	delete from Inventory
	where not exists (select * from LoginID where LoginID.CustomerID = Inventory.CustomerID)
	select @@RowCount as Deleted, 'Inventory' as FromTable

	delete from Inventory_FPS
	where not exists (select * from LoginID where LoginID.CustomerID = Inventory_FPS.CustomerID)
	select @@RowCount as Deleted, 'Inventory_FPS' as FromTable

	--delete from SteamUserIDMap
	--where not exists (select * from LoginID where LoginID.CustomerID = SteamUserIDMap.CustomerID)
	--select @@RowCount as Deleted, 'SteamUserIDMap' as FromTable

	--delete from GamersfirstUserIDMap
	--where not exists (select * from LoginID where LoginID.CustomerID = GamersfirstUserIDMap.CustomerID)
	--select @@RowCount as Deleted, 'GamersfirstUserIDMap' as FromTable
	
	-- purge inventory
	declare @InvCleanDate datetime = DATEADD(day, -30, GETDATE())
	delete from Inventory where LeasedUntil<@InvCleanDate
	delete from Inventory_FPS where LeasedUntil<@InvCleanDate
	
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[DBG_RegisterIISCall]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[DBG_RegisterIISCall]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[DBG_RegisterIISCall]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[DBG_RegisterIISCall]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [DBG_RegisterIISCall]
-- ----------------------------



CREATE PROCEDURE [dbo].[DBG_RegisterIISCall]
	@in_API	varchar(128),
	@in_BytesIn int,
	@in_BytesOut int
AS
BEGIN
	SET NOCOUNT ON;
	
	select 0 as ResultCode
	/*
	update DBG_IISApiStats set Hits=Hits+1, BytesIn=BytesIn+@in_BytesIn, BytesOut=BytesOut+@in_BytesOut where API=@in_API
	if(@@ROWCOUNT = 0) begin
		insert into DBG_IISApiStats	values (
			@in_API,
			1,
			@in_BytesIn,
			@in_BytesOut
		)
	end
	*/

	return
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[DeleteFunc]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[DeleteFunc]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[DeleteFunc]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[DeleteFunc]
-- ----------------------------
CREATE PROCEDURE [dbo].[DeleteFunc]
AS
BEGIN
DELETE UsersInventory WHERE ItemID=101324 
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[DeleteRows]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[DeleteRows]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[DeleteRows]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[DeleteRows]
-- ----------------------------
CREATE PROCEDURE [dbo].[DeleteRows]
AS
BEGIN
DELETE UsersInventory
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[FindItem]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[FindItem]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[FindItem]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[FindItem]
-- ----------------------------
CREATE PROCEDURE [dbo].[FindItem]
AS
BEGIN
SELECT * from UsersInventory where ItemID = 101088
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[FN_ADD_SECURITY_LOG]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[FN_ADD_SECURITY_LOG]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[FN_ADD_SECURITY_LOG]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[FN_ADD_SECURITY_LOG]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [FN_ADD_SECURITY_LOG]
-- ----------------------------

-- =============================================
-- Author:		Name
-- Create date: 
-- Description:	
-- =============================================
CREATE PROCEDURE [dbo].[FN_ADD_SECURITY_LOG] 
	-- Add the parameters for the stored procedure here
	@EventID int,
	@IP varchar(64),
	@CustomerID int,
	@EventData varchar(256)
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	INSERT INTO SecurityLog
		(EventID, Date, IP, CustomerID, EventData) 
	VALUES 
		(@EventID, GETDATE(), @IP, @CustomerID, @EventData)

END






GO

-- ----------------------------
-- Procedure structure for [dbo].[FN_AddItemToUser]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[FN_AddItemToUser]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[FN_AddItemToUser]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[FN_AddItemToUser]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [FN_AddItemToUser]
-- ----------------------------

CREATE PROCEDURE [dbo].[FN_AddItemToUser]
	@in_CustomerID int,
	@in_ItemID int,
	@in_ExpDays int
AS
BEGIN
	SET NOCOUNT ON;

	declare @InventoryID bigint = 0
	declare @LeasedUntil datetime
	declare @CurDate datetime = GETDATE()
	
	-- check if this is stackable item, if so - get buying stack size.
	-- stackable item defined where NumClips>0, Quantity is ClipSize
	declare @BuyStackSize int = 1
	select @BuyStackSize=ClipSize from Items_Weapons where ItemID=@in_ItemID and NumClips>0
	
	-- see if we already have that item in inventory without modification vars
	select @InventoryID=InventoryID, @LeasedUntil=LeasedUntil from UsersInventory 
		where (CustomerID=@in_CustomerID and CharID=0 and ItemID=@in_ItemID and Var1<0)
	if(@InventoryID = 0) 
	begin
		INSERT INTO UsersInventory (
			CustomerID,
			CharID,
			ItemID, 
			LeasedUntil, 
			Quantity
		)
		VALUES (
			@in_CustomerID,
			0,
			@in_ItemID,
			DATEADD(day, @in_ExpDays, @CurDate),
			@BuyStackSize
		)
		return
	end
       
	if(@LeasedUntil < @CurDate)
		set @LeasedUntil = DATEADD(day, @in_ExpDays, @CurDate)
	else
		set @LeasedUntil = DATEADD(day, @in_ExpDays, @LeasedUntil)
		
	if(@LeasedUntil > '2020-1-1')
		set @LeasedUntil = '2020-1-1'

	-- all items is stackable by default
	UPDATE UsersInventory SET 
		LeasedUntil=@LeasedUntil,
		Quantity=(Quantity+@BuyStackSize)
	WHERE InventoryID=@InventoryID

	return
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[FN_AlterUserGP]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[FN_AlterUserGP]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[FN_AlterUserGP]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[FN_AlterUserGP]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [FN_AlterUserGP]
-- ----------------------------

CREATE PROCEDURE [dbo].[FN_AlterUserGP]
	@in_CustomerID int,
	@in_GP int,
	@in_Reason varchar(64)
AS
BEGIN
	SET NOCOUNT ON;
	
	if(@in_GP = 0)
		return
		
	declare @GamePoints int = 0
	select @GamePoints=GamePoints from UsersData where CustomerID=@in_CustomerID

	insert into DBG_GPTransactions (
		CustomerID,
		TransactionTime,
		Amount,
		Reason,
		Previous
	) values (
		@in_CustomerID,
		GETDATE(),
		@in_GP,
		@in_Reason,
		@GamePoints
	)
	
	update UsersData set GamePoints=(GamePoints + @in_GP) where CustomerID=@in_CustomerID

END






GO

-- ----------------------------
-- Procedure structure for [dbo].[FN_BackpackValidateItem]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[FN_BackpackValidateItem]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[FN_BackpackValidateItem]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[FN_BackpackValidateItem]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [FN_BackpackValidateItem]
-- ----------------------------

CREATE PROCEDURE [dbo].[FN_BackpackValidateItem] 
	@in_CharID int,
	@in_ItemID int,
	@in_EquipIdx int
AS
BEGIN
	SET NOCOUNT ON;
	
	if @in_ItemID = 0
		return 0

	if not exists (SELECT ItemID from UsersBackpack WHERE CharID=@in_CharID and ItemID=@in_ItemID and LeasedUntil>GETDATE()) begin
		return 0
	end

	-- item is valid
	return @in_ItemID
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[FN_CreateMD5Password]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[FN_CreateMD5Password]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[FN_CreateMD5Password]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[FN_CreateMD5Password]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [FN_CreateMD5Password]
-- ----------------------------

CREATE PROCEDURE [dbo].[FN_CreateMD5Password]
	@in_Password varchar(100),
	@out_MD5 varchar(32) OUTPUT
AS
BEGIN
	SET NOCOUNT ON;

	declare @PASSWORD_SALT varchar(100) = 'g5sc4gs1fz0h'
	-- set @out_MD5 = @in_Password
  set @out_MD5 = SUBSTRING(master.dbo.fn_varbintohexstr(HashBytes('md5', @PASSWORD_SALT + @in_Password)), 3, 999)
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[FN_LevelUpBonus]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[FN_LevelUpBonus]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[FN_LevelUpBonus]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[FN_LevelUpBonus]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [FN_LevelUpBonus]
-- ----------------------------

CREATE PROCEDURE [dbo].[FN_LevelUpBonus] 
       @in_CustomerID int,
       @in_LevelUp int
AS
BEGIN
	SET NOCOUNT ON;

	declare @gd int = 0 -- level up bonus
	declare @sp int = 1 -- always give at least one SP

	-- not implemented yet

END






GO

-- ----------------------------
-- Procedure structure for [dbo].[GetChars]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[GetChars]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[GetChars]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[GetChars]
-- ----------------------------
CREATE PROCEDURE [dbo].[GetChars]
AS
BEGIN
select * from UsersChars WHERE Gamertag='44the26'
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[GetHackChars]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[GetHackChars]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[GetHackChars]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[GetHackChars]
-- ----------------------------
CREATE PROCEDURE [dbo].[GetHackChars]
AS
BEGIN
-- fucked script. we will used.
	select * from UsersData where AccountStatus=200
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[GetLoginLogByMac]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[GetLoginLogByMac]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[GetLoginLogByMac]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[GetLoginLogByMac]
-- ----------------------------
CREATE PROCEDURE [dbo].[GetLoginLogByMac]
AS
BEGIN
select * from LoginLog WHERE CustomerID=1129177
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[GetPremiumExpireTime]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[GetPremiumExpireTime]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[GetPremiumExpireTime]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[GetPremiumExpireTime]
-- ----------------------------
CREATE PROCEDURE [dbo].[GetPremiumExpireTime]
AS
BEGIN
--UPDATE UsersData SET PremiumExpireDate=GETUTCDATE () WHERE CustomerID=1013668
	declare @SecToRevive int
	select -- second, 1 day * 30
		@SecToRevive=DATEDIFF (second, GETUTCDATE (), DATEADD (SECOND, 0, PremiumExpireDate))
	from UsersData where CustomerID=1013668
select 0 as ResultCode, @SecToRevive as ResultMsg
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[GetPremiumUsers]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[GetPremiumUsers]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[GetPremiumUsers]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[GetPremiumUsers]
-- ----------------------------
CREATE PROCEDURE [dbo].[GetPremiumUsers]
AS
BEGIN
select * from UsersData WHERE IsPremium=1
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[GETVALUECHARLOG]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[GETVALUECHARLOG]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[GETVALUECHARLOG]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[GETVALUECHARLOG]
-- ----------------------------
CREATE PROCEDURE [dbo].[GETVALUECHARLOG]
AS
BEGIN
select * from CharsLog WHERE CustomerID=1064293
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[Punisher]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[Punisher]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[Punisher]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[Punisher]
-- ----------------------------
CREATE PROCEDURE [dbo].[Punisher]
AS
BEGIN
update UsersData set ReportCount=0
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[ScanWeaPons]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[ScanWeaPons]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[ScanWeaPons]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[ScanWeaPons]
-- ----------------------------
CREATE PROCEDURE [dbo].[ScanWeaPons]
AS
BEGIN
select * from UsersInventory WHERE CustomerID=1088952
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[SetCharsPos]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[SetCharsPos]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[SetCharsPos]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[SetCharsPos]
-- ----------------------------
CREATE PROCEDURE [dbo].[SetCharsPos]
AS
BEGIN
	update UsersChars set GamePos='7017.002 42.803 5894.719 230' WHERE Gamertag='nookud2'
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[SetRepair]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[SetRepair]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[SetRepair]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[SetRepair]
-- ----------------------------
CREATE PROCEDURE [dbo].[SetRepair]
AS
BEGIN
 update UsersInventory set Repair=100
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[sp_alterdiagram]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[sp_alterdiagram]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[sp_alterdiagram]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[sp_alterdiagram]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [sp_alterdiagram]
-- ----------------------------

	CREATE PROCEDURE [dbo].[sp_alterdiagram]
	(
		@diagramname 	sysname,
		@owner_id	int	= null,
		@version 	int,
		@definition 	varbinary(max)
	)
	WITH EXECUTE AS 'dbo'
	AS
	BEGIN
		set nocount on
	
		declare @theId 			int
		declare @retval 		int
		declare @IsDbo 			int
		
		declare @UIDFound 		int
		declare @DiagId			int
		declare @ShouldChangeUID	int
	
		if(@diagramname is null)
		begin
			RAISERROR ('Invalid ARG', 16, 1)
			return -1
		end
	
		execute as caller;
		select @theId = DATABASE_PRINCIPAL_ID();	 
		select @IsDbo = IS_MEMBER(N'db_owner'); 
		if(@owner_id is null)
			select @owner_id = @theId;
		revert;
	
		select @ShouldChangeUID = 0
		select @DiagId = diagram_id, @UIDFound = principal_id from dbo.sysdiagrams where principal_id = @owner_id and name = @diagramname 
		
		if(@DiagId IS NULL or (@IsDbo = 0 and @theId <> @UIDFound))
		begin
			RAISERROR ('Diagram does not exist or you do not have permission.', 16, 1);
			return -3
		end
	
		if(@IsDbo <> 0)
		begin
			if(@UIDFound is null or USER_NAME(@UIDFound) is null) -- invalid principal_id
			begin
				select @ShouldChangeUID = 1 ;
			end
		end

		-- update dds data			
		update dbo.sysdiagrams set definition = @definition where diagram_id = @DiagId ;

		-- change owner
		if(@ShouldChangeUID = 1)
			update dbo.sysdiagrams set principal_id = @theId where diagram_id = @DiagId ;

		-- update dds version
		if(@version is not null)
			update dbo.sysdiagrams set version = @version where diagram_id = @DiagId ;

		return 0
	END






GO

-- ----------------------------
-- Procedure structure for [dbo].[sp_creatediagram]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[sp_creatediagram]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[sp_creatediagram]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[sp_creatediagram]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [sp_creatediagram]
-- ----------------------------

	CREATE PROCEDURE [dbo].[sp_creatediagram]
	(
		@diagramname 	sysname,
		@owner_id		int	= null, 	
		@version 		int,
		@definition 	varbinary(max)
	)
	WITH EXECUTE AS 'dbo'
	AS
	BEGIN
		set nocount on
	
		declare @theId int
		declare @retval int
		declare @IsDbo	int
		declare @userName sysname
		if(@version is null or @diagramname is null)
		begin
			RAISERROR (N'E_INVALIDARG', 16, 1);
			return -1
		end
	
		execute as caller;
		select @theId = DATABASE_PRINCIPAL_ID(); 
		select @IsDbo = IS_MEMBER(N'db_owner');
		revert; 
		
		if @owner_id is null
		begin
			select @owner_id = @theId;
		end
		else
		begin
			if @theId <> @owner_id
			begin
				if @IsDbo = 0
				begin
					RAISERROR (N'E_INVALIDARG', 16, 1);
					return -1
				end
				select @theId = @owner_id
			end
		end
		-- next 2 line only for test, will be removed after define name unique
		if EXISTS(select diagram_id from dbo.sysdiagrams where principal_id = @theId and name = @diagramname)
		begin
			RAISERROR ('The name is already used.', 16, 1);
			return -2
		end
	
		insert into dbo.sysdiagrams(name, principal_id , version, definition)
				VALUES(@diagramname, @theId, @version, @definition) ;
		
		select @retval = @@IDENTITY 
		return @retval
	END






GO

-- ----------------------------
-- Procedure structure for [dbo].[sp_dropdiagram]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[sp_dropdiagram]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[sp_dropdiagram]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[sp_dropdiagram]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [sp_dropdiagram]
-- ----------------------------

	CREATE PROCEDURE [dbo].[sp_dropdiagram]
	(
		@diagramname 	sysname,
		@owner_id	int	= null
	)
	WITH EXECUTE AS 'dbo'
	AS
	BEGIN
		set nocount on
		declare @theId 			int
		declare @IsDbo 			int
		
		declare @UIDFound 		int
		declare @DiagId			int
	
		if(@diagramname is null)
		begin
			RAISERROR ('Invalid value', 16, 1);
			return -1
		end
	
		EXECUTE AS CALLER;
		select @theId = DATABASE_PRINCIPAL_ID();
		select @IsDbo = IS_MEMBER(N'db_owner'); 
		if(@owner_id is null)
			select @owner_id = @theId;
		REVERT; 
		
		select @DiagId = diagram_id, @UIDFound = principal_id from dbo.sysdiagrams where principal_id = @owner_id and name = @diagramname 
		if(@DiagId IS NULL or (@IsDbo = 0 and @UIDFound <> @theId))
		begin
			RAISERROR ('Diagram does not exist or you do not have permission.', 16, 1)
			return -3
		end
	
		delete from dbo.sysdiagrams where diagram_id = @DiagId;
	
		return 0;
	END






GO

-- ----------------------------
-- Procedure structure for [dbo].[sp_helpdiagramdefinition]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[sp_helpdiagramdefinition]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[sp_helpdiagramdefinition]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[sp_helpdiagramdefinition]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [sp_helpdiagramdefinition]
-- ----------------------------

	CREATE PROCEDURE [dbo].[sp_helpdiagramdefinition]
	(
		@diagramname 	sysname,
		@owner_id	int	= null 		
	)
	WITH EXECUTE AS N'dbo'
	AS
	BEGIN
		set nocount on

		declare @theId 		int
		declare @IsDbo 		int
		declare @DiagId		int
		declare @UIDFound	int
	
		if(@diagramname is null)
		begin
			RAISERROR (N'E_INVALIDARG', 16, 1);
			return -1
		end
	
		execute as caller;
		select @theId = DATABASE_PRINCIPAL_ID();
		select @IsDbo = IS_MEMBER(N'db_owner');
		if(@owner_id is null)
			select @owner_id = @theId;
		revert; 
	
		select @DiagId = diagram_id, @UIDFound = principal_id from dbo.sysdiagrams where principal_id = @owner_id and name = @diagramname;
		if(@DiagId IS NULL or (@IsDbo = 0 and @UIDFound <> @theId ))
		begin
			RAISERROR ('Diagram does not exist or you do not have permission.', 16, 1);
			return -3
		end

		select version, definition FROM dbo.sysdiagrams where diagram_id = @DiagId ; 
		return 0
	END






GO

-- ----------------------------
-- Procedure structure for [dbo].[sp_helpdiagrams]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[sp_helpdiagrams]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[sp_helpdiagrams]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[sp_helpdiagrams]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [sp_helpdiagrams]
-- ----------------------------

	CREATE PROCEDURE [dbo].[sp_helpdiagrams]
	(
		@diagramname sysname = NULL,
		@owner_id int = NULL
	)
	WITH EXECUTE AS N'dbo'
	AS
	BEGIN
		DECLARE @user sysname
		DECLARE @dboLogin bit
		EXECUTE AS CALLER;
			SET @user = USER_NAME();
			SET @dboLogin = CONVERT(bit,IS_MEMBER('db_owner'));
		REVERT;
		SELECT
			[Database] = DB_NAME(),
			[Name] = name,
			[ID] = diagram_id,
			[Owner] = USER_NAME(principal_id),
			[OwnerID] = principal_id
		FROM
			sysdiagrams
		WHERE
			(@dboLogin = 1 OR USER_NAME(principal_id) = @user) AND
			(@diagramname IS NULL OR name = @diagramname) AND
			(@owner_id IS NULL OR principal_id = @owner_id)
		ORDER BY
			4, 5, 1
	END






GO

-- ----------------------------
-- Procedure structure for [dbo].[sp_renamediagram]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[sp_renamediagram]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[sp_renamediagram]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[sp_renamediagram]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [sp_renamediagram]
-- ----------------------------

	CREATE PROCEDURE [dbo].[sp_renamediagram]
	(
		@diagramname 		sysname,
		@owner_id		int	= null,
		@new_diagramname	sysname
	
	)
	WITH EXECUTE AS 'dbo'
	AS
	BEGIN
		set nocount on
		declare @theId 			int
		declare @IsDbo 			int
		
		declare @UIDFound 		int
		declare @DiagId			int
		declare @DiagIdTarg		int
		declare @u_name			sysname
		if((@diagramname is null) or (@new_diagramname is null))
		begin
			RAISERROR ('Invalid value', 16, 1);
			return -1
		end
	
		EXECUTE AS CALLER;
		select @theId = DATABASE_PRINCIPAL_ID();
		select @IsDbo = IS_MEMBER(N'db_owner'); 
		if(@owner_id is null)
			select @owner_id = @theId;
		REVERT;
	
		select @u_name = USER_NAME(@owner_id)
	
		select @DiagId = diagram_id, @UIDFound = principal_id from dbo.sysdiagrams where principal_id = @owner_id and name = @diagramname 
		if(@DiagId IS NULL or (@IsDbo = 0 and @UIDFound <> @theId))
		begin
			RAISERROR ('Diagram does not exist or you do not have permission.', 16, 1)
			return -3
		end
	
		-- if((@u_name is not null) and (@new_diagramname = @diagramname))	-- nothing will change
		--	return 0;
	
		if(@u_name is null)
			select @DiagIdTarg = diagram_id from dbo.sysdiagrams where principal_id = @theId and name = @new_diagramname
		else
			select @DiagIdTarg = diagram_id from dbo.sysdiagrams where principal_id = @owner_id and name = @new_diagramname
	
		if((@DiagIdTarg is not null) and  @DiagId <> @DiagIdTarg)
		begin
			RAISERROR ('The name is already used.', 16, 1);
			return -2
		end		
	
		if(@u_name is null)
			update dbo.sysdiagrams set [name] = @new_diagramname, principal_id = @theId where diagram_id = @DiagId
		else
			update dbo.sysdiagrams set [name] = @new_diagramname where diagram_id = @DiagId
		return 0
	END






GO

-- ----------------------------
-- Procedure structure for [dbo].[sp_upgraddiagrams]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[sp_upgraddiagrams]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[sp_upgraddiagrams]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[sp_upgraddiagrams]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [sp_upgraddiagrams]
-- ----------------------------

	CREATE PROCEDURE [dbo].[sp_upgraddiagrams]
	AS
	BEGIN
		IF OBJECT_ID(N'dbo.sysdiagrams') IS NOT NULL
			return 0;
	
		CREATE TABLE dbo.sysdiagrams
		(
			name sysname NOT NULL,
			principal_id int NOT NULL,	-- we may change it to varbinary(85)
			diagram_id int PRIMARY KEY IDENTITY,
			version int,
	
			definition varbinary(max)
			CONSTRAINT UK_principal_name UNIQUE
			(
				principal_id,
				name
			)
		);


		/* Add this if we need to have some form of extended properties for diagrams */
		/*
		IF OBJECT_ID(N'dbo.sysdiagram_properties') IS NULL
		BEGIN
			CREATE TABLE dbo.sysdiagram_properties
			(
				diagram_id int,
				name sysname,
				value varbinary(max) NOT NULL
			)
		END
		*/

		IF OBJECT_ID(N'dbo.dtproperties') IS NOT NULL
		begin
			insert into dbo.sysdiagrams
			(
				[name],
				[principal_id],
				[version],
				[definition]
			)
			select	 
				convert(sysname, dgnm.[uvalue]),
				DATABASE_PRINCIPAL_ID(N'dbo'),			-- will change to the sid of sa
				0,							-- zero for old format, dgdef.[version],
				dgdef.[lvalue]
			from dbo.[dtproperties] dgnm
				inner join dbo.[dtproperties] dggd on dggd.[property] = 'DtgSchemaGUID' and dggd.[objectid] = dgnm.[objectid]	
				inner join dbo.[dtproperties] dgdef on dgdef.[property] = 'DtgSchemaDATA' and dgdef.[objectid] = dgnm.[objectid]
				
			where dgnm.[property] = 'DtgSchemaNAME' and dggd.[uvalue] like N'_EA3E6268-D998-11CE-9454-00AA00A3F36E_' 
			return 2;
		end
		return 1;
	END






GO

-- ----------------------------
-- Procedure structure for [dbo].[TEMP_AddGPToUser]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[TEMP_AddGPToUser]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[TEMP_AddGPToUser]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[TEMP_AddGPToUser]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [TEMP_AddGPToUser]
-- ----------------------------

CREATE PROCEDURE [dbo].[TEMP_AddGPToUser]
	@in_email varchar(128),
	@in_GP int
AS
BEGIN
	SET NOCOUNT ON;
	
	--
	-- used in account.thewarz.com/admin/gpadd
	-- 
	
	declare @CustomerID int = 0
	select @CustomerID=CustomerID from dbo.Accounts where email=@in_email
	if(@@ROWCOUNT = 0) begin
		select 0 as 'CustomerID'
		return
	end
		
	declare @GamePoints int = 0
	select @GamePoints=GamePoints from UsersData where CustomerID=@CustomerID

	insert into DBG_GPTransactions (
		CustomerID,
		TransactionTime,
		Amount,
		Reason,
		Previous
	) values (
		@CustomerID,
		GETDATE(),
		@in_GP,
		'ADMIN_ADD',
		@GamePoints
	)
	
	update UsersData set GamePoints=(GamePoints + @in_GP) where CustomerID=@CustomerID
	select @CustomerID as 'CustomerID', (@GamePoints+@in_GP) as 'GamePoints'

END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Account_ChangePassword]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Account_ChangePassword]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WEB_Account_ChangePassword]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Account_ChangePassword]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [[WEB_Account_ChangePassword]]
-- ----------------------------

CREATE PROCEDURE [dbo].[WEB_Account_ChangePassword]
	@in_CustomerID int,
	@in_SessionID nvarchar(50),
	@in_Password nvarchar(50),
	@in_NewPassword nvarchar(50)
AS
BEGIN
	SET NOCOUNT ON;
	
	------------------------------------------------
	-- Verfiy
	-- 1. Verify SessionID
	-- 2. Verify Current Password
	------------------------------------------------
	
	-- Verify SessionID
	IF Not Exists (SELECT * FROM WebLoginSessions WHERE CustomerID=@in_CustomerID and SessionID=@in_SessionID)
	BEGIN
		select 7 as ResultCode, 'Invalid SessionID' as ResultMsg
		return;
	END
	
	-- Verify Current Password
	IF Not Exists (SELECT * FROM Accounts WHERE CustomerID=@in_CustomerID and MD5Password=@in_Password)
	BEGIN
		select 8 as ResultCode, 'Invalid Current Password' as ResultMsg
		return;
	END
	
	------------------------------------------------
	-- Change Password
	------------------------------------------------
	UPDATE Accounts SET MD5Password=@in_NewPassword WHERE CustomerID=@in_CustomerID
	
	------------------------------------------------
	-- Clear Game and Web Session
	------------------------------------------------
	UPDATE LoginSessions SET SessionID=0 WHERE CustomerID=@in_CustomerID
	UPDATE WebLoginSessions SET SessionID=0 WHERE CustomerID=@in_CustomerID
	
	select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Account_Create]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Account_Create]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WEB_Account_Create]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Account_Create]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WEB_Account_Create]
-- ----------------------------

CREATE PROCEDURE [dbo].[WEB_Account_Create]
	@in_IP varchar(64),
	@in_Email varchar(128),
	@in_Password varchar(64)
AS
BEGIN
	SET NOCOUNT ON;
	
	--
	-- NOTE: add new ResultCodes to updater CUpdater::CreateAccThreadEntry
	--
	
-- validate that email is unique
	if exists (SELECT CustomerID from Accounts with(nolock) WHERE email=@in_Email) begin
		select 2 as ResultCode, 'Email already in use' as ResultMsg;
		return;
	end
	
-- create user
	declare @MD5FromPwd varchar(100)
	exec FN_CreateMD5Password @in_Password, @MD5FromPwd OUTPUT
	INSERT INTO Accounts ( 
		email,
		MD5Password,
		dateregistered,
		ReferralID,
		lastlogindate,
		lastloginIP
	) VALUES (
		@in_EMail,
		@MD5FromPwd,
		GETDATE(),
		0,
		GETDATE(),
		@in_IP
	)

	-- get new CustomerID
	declare @CustomerID int
	SELECT @CustomerID=CustomerID from Accounts with(nolock) where email=@in_Email

-- create all needed user tables
	INSERT INTO UsersData (
		CustomerID,
		AccountType,
		dateregistered,
		IsPunisher
	) VALUES (
		@CustomerID,
		2,
		GETDATE(),
		0
	)

	-- default items and bonuses for account types
	 exec FN_AddItemToUser @CustomerID, 20174, 2000 -- hero: regular guy

	-- CBT TEST HEROES
	 exec FN_AddItemToUser @CustomerID, 20182, 2000
	 exec FN_AddItemToUser @CustomerID, 20184, 2000

	-- CBT TEST BONUSES
	--update UsersData set GamePoints=(GamePoints+10000) where CustomerID=@CustomerID
	
	-- success
	select 0 as ResultCode
	select @CustomerID as CustomerID, 0 as 'AccountType'

	return
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Account_Login]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Account_Login]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WEB_Account_Login]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Account_Login]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [[WEB_Account_Login]]
-- ----------------------------

CREATE PROCEDURE [dbo].[WEB_Account_Login]
	@in_IP varchar(100),
	@in_EMail varchar(100), 
	@in_Password varchar(100)
AS
BEGIN
	SET NOCOUNT ON;
	
	declare @CustomerID int
	declare @MD5Password varchar(100)
	declare @AccountStatus int = 0	-- this is Accounts.AccountStatus

	-- this call is always valid
	select 0 as ResultCode
	
	-- search for record with username
	SELECT 
		@CustomerID=CustomerID,
		@MD5Password=MD5Password,
		@AccountStatus=AccountStatus
	FROM Accounts
	WHERE email=@in_Email
	if (@@RowCount = 0) begin
		select
			1 as LoginResult,
			0 as CustomerID,
			0 as AccountStatus
		return
	end

	-- check MD5 password
	declare @MD5FromPwd varchar(100)
	exec FN_CreateMD5Password @in_Password, @MD5FromPwd OUTPUT
	if(@MD5Password <> @MD5FromPwd) begin
		select
			2 as LoginResult,
			0 as CustomerID,
			0 as AccountStatus
		return
	end
	
	-- check if deleted account because of refund
	if(@AccountStatus = 999) begin
		select
			3 as LoginResult,
			0 as CustomerID,
			999 as AccountStatus
		return
	end
	
	-- perform actual login
	exec Web_Account_Login_Exec @in_IP, @CustomerID
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Account_Login_Exec]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Account_Login_Exec]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WEB_Account_Login_Exec]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Account_Login_Exec]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ACCOUNT_LOGIN_EXEC]
-- ----------------------------

CREATE PROCEDURE [dbo].[WEB_Account_Login_Exec]
	@in_IP varchar(100),
	@CustomerID int
AS
BEGIN
	SET NOCOUNT ON;
	
	--
	-- helper function that perform actual user login
	--
	
	declare @IsDeveloper int = 0
	declare @AccountStatus int
	declare @DateActiveUntil datetime
	declare @BanExpireDate datetime
	declare @lastgamedate datetime
	declare @GameServerId int
	select
		@AccountStatus=AccountStatus, 
		@IsDeveloper=IsDeveloper, 
		@DateActiveUntil=DateActiveUntil,
		@BanExpireDate=BanExpireDate,
		@GameServerId=GameServerId, 
		@lastgamedate=lastgamedate
		from UsersData with(nolock) where CustomerID=@CustomerID
	if(@@ROWCOUNT = 0) begin
		select
			5 as LoginResult,
			@CustomerID as CustomerID,
			0 as AccountStatus,
			0 as SessionID,
			0 as IsDeveloper
		return
	end

	-- check if account time expired
	if(GETDATE() > @DateActiveUntil) begin
		select
			4 as LoginResult,
			@CustomerID as CustomerID,
			300 as AccountStatus,	-- special 'TimeExpired' code
			0 as SessionID,
			0 as IsDeveloper
		return
	end 

	-- status equal to 201 means temporary ban
	if (@AccountStatus = 201) begin
		declare @BanExpireMin int = DATEDIFF(mi, GETDATE(), @BanExpireDate)
		if(@BanExpireMin > 0) begin
			select
				3 as LoginResult,
				@CustomerID as CustomerID,
				@AccountStatus as AccountStatus,
				@BanExpireMin as SessionID,
				0 as IsDeveloper
			return
		end
		else
		begin
			-- unban player
			set @AccountStatus = 100
			update dbo.UsersData set AccountStatus=@AccountStatus where CustomerID=@CustomerID
		end
	end

	if (@AccountStatus >= 200) begin
		select
			3 as LoginResult,
			@CustomerID as CustomerID,
			@AccountStatus as AccountStatus,
			0 as SessionID,
			0 as IsDeveloper
		return
	end
	
	-- check if game is still active or 90sec passed from last update (COPYPASTE_GAMECHECK, search for others)
	if(@GameServerId > 0 and DATEDIFF(second, @lastgamedate, GETDATE()) < 90) begin
		select
			0 as LoginResult,
			@CustomerID as CustomerID,
			70 as AccountStatus,	-- game still active code
			0 as SessionID,
			0 as IsDeveloper
		return
	end
	
	-- update session key/id
	declare @SessionKey varchar(50) = NEWID()
	declare @SessionID int = checksum(@SessionKey)
	if exists (SELECT CustomerID FROM WebLoginSessions WHERE CustomerID = @CustomerID)
	begin
		UPDATE WebLoginSessions SET 
			SessionKey=@SessionKey, 
			SessionID=@SessionID,
			LoginIP=@in_IP, 
			TimeLogged=GETDATE(), 
			TimeUpdated=GETDATE()
		WHERE CustomerID=@CustomerID
	end
	else
	begin
		INSERT INTO WebLoginSessions
			(CustomerID, SessionKey, SessionID, LoginIP, TimeLogged, TimeUpdated)
		VALUES 
			(@CustomerID, @SessionKey, @SessionID, @in_IP, GETDATE(), GETDATE())
	end

	-- update other tables
	--UPDATE Accounts SET 
	--	lastlogindate=GETDATE(), 
	--	lastloginIP=@in_IP
	--WHERE CustomerID=@CustomerID
	
	--INSERT INTO Logins 
	--	(CustomerID, LoginTime, IP, LoginSource) 
	--VALUES 
	--	(@CustomerID, GETDATE(), @in_IP, 0)

	-- return session info
	SELECT 
		0 as LoginResult,
		@CustomerID as CustomerID,
		@AccountStatus as AccountStatus,
		@SessionID as SessionID,
		@IsDeveloper as IsDeveloper
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Account_View]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Account_View]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WEB_Account_View]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Account_View]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WEB_Account_View]
-- ----------------------------

CREATE PROCEDURE [dbo].[WEB_Account_View]
	@in_CustomerID int,
	@in_SessionID nvarchar(50)
AS
BEGIN
	SET NOCOUNT ON;
	
	-- Verify SessionID
	IF Not Exists (SELECT * FROM WebLoginSessions WHERE CustomerID=@in_CustomerID and SessionID=@in_SessionID)
	BEGIN
		select 7 as ResultCode, 'Invalid SessionID' as ResultMsg
		return;
	END
	
	-- Get FacebookPoints from Application_Revive
	Declare @FacebookPoints int
	SELECT @FacebookPoints = Application_Revive.Point FROM Application_Revive where CustomerID = @in_CustomerID
	IF @FacebookPoints IS NULL
		SET @FacebookPoints = 0
		
	-- Get LoginCount from LoginLog
	Declare @LoginCount int
	SELECT @LoginCount = COUNT(*) FROM LoginLog where CustomerID = @in_CustomerID
	IF @LoginCount IS NULL
		SET @LoginCount = 0
		
	select 0 as ResultCode
	
	SELECT 
		ac.email As Email,
		ac.CustomerID As CustomerID,
		ud.AccountStatus As AccountStatus,
		ud.GamePoints As GamePoints,
		ud.GameDollars As GameDollars,
		@FacebookPoints As FacebookPoints,
		@LoginCount As LoginCount,
		ac.lastloginIP As LastIP
	FROM Accounts ac
	JOIN UsersData ud
		on ud.CustomerID = ac.CustomerID
	where ac.CustomerID = @in_CustomerID
	
	
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WEB_ADMIN_BanUser]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WEB_ADMIN_BanUser]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WEB_ADMIN_BanUser]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WEB_ADMIN_BanUser]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [ADMIN_BanUser]
-- ----------------------------

CREATE PROCEDURE [dbo].[WEB_ADMIN_BanUser]
	@in_CustomerID int,
	@in_BanReason nvarchar(256)
AS
BEGIN
	SET NOCOUNT ON;
	if(LEN(@in_BanReason) < 4) begin
		select 'GIVE PROPER REASON'
		return
	end

	declare @email varchar(128)
	select @email=email from dbo.Accounts where CustomerID=@in_CustomerID

	-- do not ban multiple times
	declare @AccountStatus int = 0
	declare @AccountType int = 0
	declare @BanCount int = 0
	declare @BanReason nvarchar(512)
	select 
		@AccountStatus=AccountStatus, 
		@accountType=AccountType,
		@BanCount=BanCount, 
		@BanReason=BanReason 
	from UsersData where CustomerID=@in_CustomerID
	if(@AccountStatus = 200 or @AccountStatus = 201) begin
		select 0 as ResultCode, 'already banned' as ResultMsg, @email as 'email'
		return
	end

	-- clear his login session
	update dbo.LoginSessions set SessionID=0 where CustomerID=@in_CustomerID
	
	-- set his all alive chars to respawned mode
	update dbo.UsersChars set Alive=2 where CustomerID=@in_CustomerID and Alive=1

	if(@BanReason is null) set @BanReason = @in_BanReason
	else                   set @BanReason = @in_BanReason + ', ' + @BanReason

	-- guest ban, permament
	if(@AccountType	= 3) begin
		set @BanReason = '[GUEST_BAN] ' + @BanReason
		set @BanCount  = 99;
	end
	
	-- ban
	if(@BanCount > 0) 
	begin
		insert into dbo.DBG_BanLog values (@in_CustomerID, GETDATE(), 2000, @in_BanReason)

		update UsersData set 
			AccountStatus=200, 
			BanReason=@BanReason, 
			BanTime=GETDATE(),
			BanCount=(BanCount+1),
			BanExpireDate='2030-1-1'
			where CustomerID=@in_CustomerID

		select 0 as ResultCode, 'Permanent BAN' as ResultMsg, @email as 'email', @BanReason as 'BanReason'
		return
	end
	else
	begin
		declare @BanTime int = 30

		insert into dbo.DBG_BanLog values (@in_CustomerID, GETDATE(), @BanTime, @in_BanReason)

		update UsersData set 
			AccountStatus=200, 
			BanReason=@BanReason, 
			BanTime=GETDATE(),
			BanCount=(BanCount+1),
			BanExpireDate='2030-1-1'
			--BanExpireDate=DATEADD(DAY, @BanTime, GETDATE())
			where CustomerID=@in_CustomerID

		select 0 as ResultCode, 'temporary ban' as ResultMsg, @email as 'email', @BanReason as 'BanReason'
		return
	end
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Application_Revive]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Application_Revive]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WEB_Application_Revive]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Application_Revive]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [[WEB_Application_Revive]]
-- ----------------------------

CREATE PROCEDURE [dbo].[WEB_Application_Revive]
	@in_CustomerID int,
	@in_Point int
AS
BEGIN
	SET NOCOUNT ON;
	
	Begin Transaction
	
	-- Add Point to Table
	IF (NOT EXISTS(SELECT * FROM Application_Revive WHERE CustomerID = @in_CustomerID)) 
	BEGIN 
		INSERT INTO Application_Revive(CustomerID, Point, PointTotal) 
		VALUES(@in_CustomerID, @in_Point, @in_Point) 
	END 
	ELSE 
	BEGIN 
		
		DECLARE @curpoint int = 0
		SELECT @curpoint = Point  FROM Application_Revive WHERE CustomerID = @in_CustomerID
		
		IF @curpoint < 20
		BEGIN
			UPDATE Application_Revive 
			SET Point = Point + @in_Point, PointTotal = PointTotal + @in_Point
			WHERE CustomerID = @in_CustomerID
		END
	END 
	
	Commit Transaction
	
	select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Character_RePosition]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Character_RePosition]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WEB_Character_RePosition]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Character_RePosition]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WEB_Character_RePosition]
-- ----------------------------

CREATE PROCEDURE [dbo].[WEB_Character_RePosition]
	@in_CustomerID int,
	@in_CharID int,
	@in_SessionID nvarchar(50),
	@in_PositionNO int
AS
BEGIN
	SET NOCOUNT ON;
	
	-- Verify SessionID
	IF Not Exists (SELECT * FROM WebLoginSessions WHERE CustomerID=@in_CustomerID and SessionID=@in_SessionID)
	BEGIN
		select 7 as ResultCode, 'Invalid SessionID' as ResultMsg
		return;
	END
	
	-- Check CharID from that CustomerID
	IF Not Exists (SELECT * FROM UsersChars WHERE CustomerID=@in_CustomerID and CharID=@in_CharID)
	BEGIN
		select 7 as ResultCode, 'Invalid CharID' as ResultMsg
		return;
	END
	
	
	
	DECLARE @GamePos varchar(128) = ''
	If @in_PositionNO = 1
		Set @GamePos = '6243.836 120.151 6394.655 356'
	Else
		Set @GamePos = '2945.139 132.135 2233.694 10'
	
	-- Temporary Ban for 10 Mins.
	declare @AccountStatus int = 0
	select @AccountStatus=AccountStatus from UsersData where CustomerID=@in_CustomerID
	if(@AccountStatus = 100) begin
		update UsersData set 
				AccountStatus=201, 
				BanReason='Change Position', 
				BanTime=GETDATE(),
				BanExpireDate=DATEADD(MINUTE, 10, GETDATE())
				where CustomerID=@in_CustomerID
	end
			
	UPDATE LoginSessions set SessionID = 0 where CustomerID = @in_CustomerID
	UPDATE UsersChars Set GamePos = @GamePos where CharID = @in_CharID
	
	
	select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Character_View]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Character_View]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WEB_Character_View]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Character_View]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WEB_Character_View]
-- ----------------------------

CREATE PROCEDURE [dbo].[WEB_Character_View]
	@in_CustomerID int,
	@in_SessionID nvarchar(50)
AS
BEGIN
	SET NOCOUNT ON;
	
	-- Verify SessionID
	IF Not Exists (SELECT * FROM WebLoginSessions WHERE CustomerID=@in_CustomerID and SessionID=@in_SessionID)
	BEGIN
		select 7 as ResultCode, 'Invalid SessionID' as ResultMsg
		return;
	END
	
	select 0 as ResultCode
	SELECT 
		uc.CustomerID,
		uc.CharID,
		uc.Gamertag,
		ISNULL(cd.ClanName,'-') as ClanName
	FROM UsersChars uc
	LEFT JOIN ClanData cd on cd.ClanID = uc.ClanID
	where uc.CustomerID = @in_CustomerID
	ORDER BY uc.CharID asc
	
	
	
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Donate_Truemoney]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Donate_Truemoney]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WEB_Donate_Truemoney]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Donate_Truemoney]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WEB_Donate_Truemoney]
-- ----------------------------

CREATE PROCEDURE [dbo].[WEB_Donate_Truemoney]
	@in_CustomerID int,
	@in_GC int,
	@in_Item int,
	@in_ItemAmount int,
	@in_CardNo nvarchar(50)
AS
BEGIN
	SET NOCOUNT ON;
	
	Begin Transaction
	
	-- Add GC to Customer
	UPDATE UsersData SET GamePoints=(GamePoints+@in_GC) WHERE CustomerID = @in_CustomerID
	
	-- Add Item to Customer
	if(@in_Item != 0 and @in_ItemAmount != 0) begin
		exec FN_AddItemToUser @in_CustomerID, @in_Item, @in_ItemAmount
	end
	
	INSERT INTO DonateLogs (CustomerID,GC,Item,ItemAmount,Time, CardNo) VALUES (@in_CustomerID, @in_GC, @in_Item, @in_ItemAmount, getdate(), @in_CardNo)
	
	Commit Transaction
	
	select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Premium_AddGC]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Premium_AddGC]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WEB_Premium_AddGC]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WEB_Premium_AddGC]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WEB_Premium_AddGC]
-- ----------------------------

CREATE PROCEDURE [dbo].[WEB_Premium_AddGC]
	
AS
BEGIN
	SET NOCOUNT ON;
	
	Begin Transaction
	
	If Not Exists (SELECT * FROM PremiumReceiveGCLog WHERE Receive = convert(date, getdate()))
	BEGIN
		UPDATE UsersData SET GamePoints=GamePoints+30 WHERE CustomerID in
			(SELECT CustomerID FROM UsersData WHERE IsPremium=1 and PremiumExpireDate >= GETDATE())
		
		INSERT INTO PremiumReceiveGCLog (Receive) VALUES(GETDATE())
	END
	Commit Transaction
	
	select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WG_SetLoginSession]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WG_SetLoginSession]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WG_SetLoginSession]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WG_SetLoginSession]
-- ----------------------------
CREATE PROCEDURE [dbo].[WG_SetLoginSession]
@in_CustomerID int
AS
BEGIN
declare @SessionID int
SELECT @SessionID=SessionID FROM LoginSessions WHERE CustomerID=@in_CustomerID
update UsersData set WarGuardSessionID=@SessionID WHERE CustomerID=@in_CustomerID
select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_APPLYKEY]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_APPLYKEY]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_APPLYKEY]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_APPLYKEY]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ACCOUNT_APPLYKEY]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ACCOUNT_APPLYKEY]
	@in_CustomerID int,
	@in_SerialKey varchar(128)
AS






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_BUYPREMIUM]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_BUYPREMIUM]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_BUYPREMIUM]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_BUYPREMIUM]
-- ----------------------------
CREATE PROCEDURE [dbo].[WZ_ACCOUNT_BUYPREMIUM]
@in_CustomerID int
AS
BEGIN
	declare @GamePoints int
	declare @isPremium int
	
	SELECT @isPremium=isPremium FROM UsersData WHERE CustomerID=@in_CustomerID
	SELECT @GamePoints=GamePoints FROM UsersData WHERE CustomerID=@in_CustomerID
	if(@isPremium = 1) begin
		select 7 as ResultCode, 'Already Premium Account' as ResultMsg
		return
	end
	if(@GamePoints < 3000) begin
		select 7 as ResultCode, 'Not Enough GC' as ResultMsg
		return
	end
	
	update UsersData set GamePoints=(GamePoints-3000) WHERE CustomerID=@in_CustomerID
	update UsersData set isPremium=1 WHERE CustomerID=@in_CustomerID
	update UsersData set PremiumExpireDate=DATEADD(DAY, 30, GETDATE()) WHERE CustomerID=@in_CustomerID
	
select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_ChangePassword]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_ChangePassword]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_ChangePassword]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_ChangePassword]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ACCOUNT_ChangePassword]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ACCOUNT_ChangePassword]
	@in_CustomerID int, 
	@in_NewPassword varchar(100)
AS
BEGIN
	SET NOCOUNT ON;

	-- update new password
	declare @MD5FromPwd varchar(100)
	exec FN_CreateMD5Password @in_NewPassword, @MD5FromPwd OUTPUT
	update Accounts set MD5Password=@MD5FromPwd where CustomerID=@in_CustomerID
	
	insert into DBG_PasswordResets (
		CustomerID,
		ResetDate,
		NewPassword
	) values (
		@in_CustomerID,
		GETDATE(),
		'' --@in_NewPassword Removed DO NOT store password as plain text ! ;/
	)

	select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_CREATE]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_CREATE]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_CREATE]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_CREATE]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ACCOUNT_CREATE]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ACCOUNT_CREATE]
	@in_IP varchar(64),
	@in_Email varchar(128),
	@in_Password varchar(64),
	@in_ReferralID int,
	@in_SerialKey varchar(128),
	@in_SerialEmail varchar(128)
AS
BEGIN
	SET NOCOUNT ON;
	
	-- Close Register from Launcher
	-- select 2 as ResultCode, 'Registeration from Patcher/Launcher was closed.' as ResultMsg;
	-- return;
		
	--
	-- NOTE: add new ResultCodes to updater CUpdater::CreateAccThreadEntry
	--
	

-- check for serial key
	--declare @keyResultCode int = 99
	--declare @keyCustomerID int = 99
	--declare @keySerialType int = 99
	--exec [BreezeNet].[dbo].[BN_WarZ_SerialGetInfo]
		--	@in_SerialKey,
		--	@in_SerialEmail,
		--	@keyResultCode out,
		--	@keyCustomerID out,
		--	@keySerialType out
								
	--if(@keyResultCode <> 0 or @keyCustomerID > 0) begin
	--	select 3 as ResultCode, 'Serial not valid' as ResultMsg;
	--	return
	--end
	declare @keySerialType int = 0 -- Legend Package

-- check if that account was created and refunded before (status 999)
	declare @RefundCustomerID int = 0
	select @RefundCustomerID=CustomerID from Accounts WHERE email=@in_Email and AccountStatus=999
	if(@RefundCustomerID > 0) begin
		-- change email to some unique one so it can be used again.
		declare @dateTime varchar(128)
		set @dateTime = REPLACE(CONVERT(VARCHAR, GETDATE(),111),'/','') + REPLACE(CONVERT(VARCHAR, GETDATE(),108),':','')
		declare @refundedEmail varchar(128) = '(' + @dateTime + ') ' + @in_Email
		update Accounts set email=@refundedEmail where CustomerID=@RefundCustomerID
	end
	
-- validate that email is unique
	if exists (SELECT CustomerID from Accounts WHERE email=@in_Email) begin
		select 2 as ResultCode, 'Email already in use' as ResultMsg;
		return;
	end
	
-- create user
	declare @MD5FromPwd varchar(100)
	exec FN_CreateMD5Password @in_Password, @MD5FromPwd OUTPUT
	INSERT INTO Accounts ( 
		email,
		MD5Password,
		dateregistered,
		ReferralID,
		lastlogindate,
		lastloginIP
	) VALUES (
		@in_EMail,
		@MD5FromPwd,
		GETDATE(),
		@in_ReferralID,
		GETDATE(),
		@in_IP
	)

	-- get new CustomerID
	declare @CustomerID int
	SELECT @CustomerID=CustomerID from Accounts where email=@in_Email

-- create all needed user tables
	INSERT INTO UsersData (
		CustomerID,
		AccountType,
		dateregistered,
IsPunisher
	) VALUES (
		@CustomerID,
		'2',
		GETDATE(),
'0'
	)
	
	-- guest accounts have 48hrs play time (sync with WZ_ACCOUNT_APPLYKEY also)
	if(@keySerialType = 3) begin
		declare @DateActiveUntil datetime = DATEADD(hour, 48, GETDATE())
		update UsersData set DateActiveUntil=@DateActiveUntil where CustomerID=@CustomerID
	end
	
-- register CustomerID in BreezeNet
	-- exec [BreezeNet].[dbo].[BN_WarZ_SerialSetCustomerID] @in_SerialKey, @CustomerID

-- default items and bonuses for account types

	exec FN_AddItemToUser @CustomerID, 20174, 2000 -- hero: regular guy

	-- CBT TEST HEROES
	exec FN_AddItemToUser @CustomerID, 20182, 2000
	exec FN_AddItemToUser @CustomerID, 20184, 2000

	-- 10 of each
	--declare @i int = 0
	--while(@i < 10) begin
	--	set @i = @i + 1

	--	exec FN_AddItemToUser @CustomerID, 101306, 2000 -- Flashlight
	--	exec FN_AddItemToUser @CustomerID, 101261, 2000 -- Bandages
	--	exec FN_AddItemToUser @CustomerID, 101296, 2000 -- Can of Soda
	--	exec FN_AddItemToUser @CustomerID, 101289, 2000 -- Granola Bar
	--end

-- BONUSES for packages - do not forget to sync them with [WZ_ACCOUNT_APPLYKEY]
	
	-- success
	select 0 as ResultCode
	select @CustomerID as CustomerID, @keySerialType as 'AccountType'

	return
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_DELETE]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_DELETE]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_DELETE]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_DELETE]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ACCOUNT_DELETE]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ACCOUNT_DELETE]
	@in_CustomerID int
AS
BEGIN
	SET NOCOUNT ON;
	
	-- success
	select 0 as ResultCode
	
	delete from dbo.UsersChars where CustomerID=@in_CustomerID
	delete from dbo.UsersData where CustomerID=@in_CustomerID
	delete from dbo.UsersInventory where CustomerID=@in_CustomerID
	
	update dbo.Accounts set AccountStatus=999 where CustomerID=@in_CustomerID

	return
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_LOGIN]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_LOGIN]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_LOGIN]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_LOGIN]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ACCOUNT_LOGIN]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ACCOUNT_LOGIN]
	@in_IP varchar(100),
	@in_EMail varchar(100), 
	@in_HardwareID varchar(50),
	@in_MiniACRC varchar(50),
	@in_WarGuardCRC varchar(50),
	@in_Password varchar(100)
AS
BEGIN
	SET NOCOUNT ON;
	
	declare @CustomerID int
	declare @MD5Password varchar(100)
	declare @serverkey varchar(100)
	declare @AccountStatus int = 0	-- this is Accounts.AccountStatus

	-- this call is always valid
	select 0 as ResultCode
	-- search for record with username
	SELECT 
		@CustomerID=CustomerID,
		@MD5Password=MD5Password,
		@AccountStatus=AccountStatus
	FROM Accounts
	WHERE email=@in_Email
	if (@@RowCount = 0) begin
		select
			1 as LoginResult,
			0 as CustomerID,
			0 as AccountStatus
		return
	end

	-- check MD5 password
	declare @MD5FromPwd varchar(100)
	exec FN_CreateMD5Password @in_Password, @MD5FromPwd OUTPUT
	if(@MD5Password <> @MD5FromPwd) begin
		select
			2 as LoginResult,
			0 as CustomerID,
			0 as AccountStatus
		return
	end
	
	-- check if deleted account because of refund
	if(@AccountStatus = 999) begin
		select
			3 as LoginResult,
			0 as CustomerID,
			999 as AccountStatus
		return
	end
	
	-- perform actual login
	exec WZ_ACCOUNT_LOGIN_EXEC @in_IP, @in_HardwareID, @in_MiniACRC, @in_WarGuardCRC, @CustomerID
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_LOGIN_EXEC]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_LOGIN_EXEC]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_LOGIN_EXEC]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ACCOUNT_LOGIN_EXEC]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ACCOUNT_LOGIN_EXEC]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ACCOUNT_LOGIN_EXEC]
	@in_IP varchar(100),
	@in_HardwareID varchar(50),
	@in_MiniACRC varchar(50),
	@in_WarGuardCRC varchar(50),
	@CustomerID int
AS
BEGIN
	SET NOCOUNT ON;
	
	--
	-- helper function that perform actual user login
	--
	
	declare @IsDeveloper int = 0
	declare @AccountStatus int
	declare @DateActiveUntil datetime
	declare @BanExpireDate datetime
	declare @lastgamedate datetime
	declare @GameServerId int
	select
		@AccountStatus=AccountStatus, 
		@IsDeveloper=IsDeveloper, 
		@DateActiveUntil=DateActiveUntil,
		@BanExpireDate=BanExpireDate,
		@GameServerId=GameServerId, 
		@lastgamedate=lastgamedate
		from UsersData where CustomerID=@CustomerID
	if(@@ROWCOUNT = 0) begin
		select
			5 as LoginResult,
			@CustomerID as CustomerID,
			0 as AccountStatus,
			0 as SessionID,
			0 as IsDeveloper
		return
	end

	-- check if account time expired
	if(GETDATE() > @DateActiveUntil) begin
		select
			4 as LoginResult,
			@CustomerID as CustomerID,
			300 as AccountStatus,	-- special 'TimeExpired' code
			0 as SessionID,
			0 as IsDeveloper
		return
	end 

	-- status equal to 201 means temporary ban
	if (@AccountStatus = 201) begin
		declare @BanExpireMin int = DATEDIFF(mi, GETDATE(), @BanExpireDate)
		if(@BanExpireMin > 0) begin
			select
				3 as LoginResult,
				@CustomerID as CustomerID,
				@AccountStatus as AccountStatus,
				@BanExpireMin as SessionID,
				0 as IsDeveloper
			return
		end
		else
		begin
			-- unban player
			set @AccountStatus = 100
			update dbo.UsersData set AccountStatus=@AccountStatus where CustomerID=@CustomerID
		end
	end

	if (@AccountStatus >= 200) begin
		select
			3 as LoginResult,
			@CustomerID as CustomerID,
			@AccountStatus as AccountStatus,
			0 as SessionID,
			0 as IsDeveloper
		return
	end

	if (@AccountStatus >= 202) begin
		select
			3 as LoginResult,
			@CustomerID as CustomerID,
			@AccountStatus as AccountStatus,
			0 as SessionID,
			0 as IsDeveloper
		return
	end
	
	-- check if game is still active or 90sec passed from last update (COPYPASTE_GAMECHECK, search for others)
	if(@GameServerId > 0 and DATEDIFF(second, @lastgamedate, GETDATE()) < 90) begin
		select
			0 as LoginResult,
			@CustomerID as CustomerID,
			70 as AccountStatus,	-- game still active code
			0 as SessionID,
			0 as IsDeveloper
		return
	end
	
	IF (EXISTS(SELECT * FROM MacBan WHERE Mac = @in_HardwareID)) 
	BEGIN
		select
			3 as LoginResult,
			@CustomerID as CustomerID,
			200 as AccountStatus,
			0 as SessionID,
			0 as IsDeveloper
		return
	END

	IF (EXISTS(SELECT * FROM HWID_Ban WHERE HardwareID = @in_HardwareID)) 
	BEGIN
	update UsersData set AccountStatus=202 WHERE CustomerID=@CustomerID
	select
			3 as LoginResult,
			@CustomerID as CustomerID,
			202 as AccountStatus,
			0 as SessionID,
			0 as IsDeveloper
		return
	END	
	-- update session key/id
	declare @SessionKey varchar(50) = NEWID()
	declare @SessionID int = checksum(@SessionKey)
	if exists (SELECT CustomerID FROM LoginSessions WHERE CustomerID = @CustomerID)
	begin
		UPDATE LoginSessions SET 
			SessionKey=@SessionKey, 
			SessionID=@SessionID,
			LoginIP=@in_IP, 
			TimeLogged=GETDATE(), 
			TimeUpdated=GETDATE()
		WHERE CustomerID=@CustomerID
	end
	else
	begin
		INSERT INTO LoginSessions
			(CustomerID, SessionKey, SessionID, LoginIP, TimeLogged, TimeUpdated)
		VALUES 
			(@CustomerID, @SessionKey, @SessionID, @in_IP, GETDATE(), GETDATE())
	end

	-- update other tables
	UPDATE Accounts SET 
		lastlogindate=GETDATE(), 
		lastloginIP=@in_IP
	WHERE CustomerID=@CustomerID
	
	INSERT INTO Logins 
		(CustomerID, LoginTime, IP, LoginSource) 
	VALUES 
		(@CustomerID, GETDATE(), @in_IP, 0)
		
	-- Insert to LoginLog
	--if @in_HardwareID != ''
	--begin
		INSERT INTO LoginLog (CustomerID, ip, mac, time, MiniACRC, WarGuardCRC) VALUES (@CustomerID,@in_IP,@in_HardwareID,GETDATE(),@in_MiniACRC,@in_WarGuardCRC)
	--end
	

	-- return session info
	SELECT 
		0 as LoginResult,
		@CustomerID as CustomerID,
		@AccountStatus as AccountStatus,
		@SessionID as SessionID,
		@IsDeveloper as IsDeveloper
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_AddRentServer]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_AddRentServer]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_AddRentServer]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_AddRentServer]
-- ----------------------------
CREATE PROCEDURE [dbo].[WZ_AddRentServer]
@in_Name varchar(256),
@in_id int
AS
BEGIN
INSERT INTO RentServer
		VALUES (@in_id, @in_Name)
select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_Backpack_SRV_AddItem]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_Backpack_SRV_AddItem]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_Backpack_SRV_AddItem]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_Backpack_SRV_AddItem]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_Backpack_SRV_AddItem]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_Backpack_SRV_AddItem] 
	@in_CustomerID int,
	@in_CharID int,
	@in_Slot int,
	@in_ItemID int,
	@in_Amount int,
	@in_Var1 int,
	@in_Var2 int
AS
BEGIN
	SET NOCOUNT ON;

	-- sanity check: input
	if(@in_ItemID = 0) begin
		select 6 as ResultCode, 'add item failed#1' as ResultMsg
		return
	end

	-- sanity check, we must not have item in that slot
	declare @InventoryID bigint = 0
	select @InventoryID=InventoryID from UsersInventory with(nolock) where CharID=@in_CharID and BackpackSlot=@in_Slot
	if(@InventoryID > 0) begin
		select 6 as ResultCode, 'add item failed#2' as ResultMsg
		return
	end

	INSERT INTO UsersInventory (
		CustomerID,
		CharID,
		BackpackSlot,
		ItemID,
		LeasedUntil, 
		Quantity,
		Var1,
		Var2,
repair
	)
	VALUES (
		@in_CustomerID,
		@in_CharID,
		@in_Slot,
		@in_ItemID,
		DATEADD(day, 2000, GETDATE()),
		@in_Amount,
		@in_Var1,
		@in_Var2,
100
	)
	set @InventoryID = SCOPE_IDENTITY()
	
	select 0 as ResultCode
	select @InventoryID as 'InventoryID'
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_Backpack_SRV_AlterItem]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_Backpack_SRV_AlterItem]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_Backpack_SRV_AlterItem]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_Backpack_SRV_AlterItem]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_Backpack_SRV_AlterItem]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_Backpack_SRV_AlterItem] 
	@in_CustomerID int,
	@in_CharID int,
	@in_Slot int,
	@in_ItemID int,
	@in_Amount int,
	@in_Var1 int,
	@in_Var2 int
AS
BEGIN
	-- SET NOCOUNT ON;

	-- sanity check: input
	if(@in_ItemID = 0) begin
		select 6 as ResultCode, 'add item failed#1' as ResultMsg
		return
	end

	update UsersInventory set 
		ItemID=@in_ItemID,
		Quantity=@in_Amount,
		Var1=@in_Var1,
		Var2=@in_Var2
	where CharID=@in_CharID and BackpackSlot=@in_Slot
	
	if(@@ROWCOUNT = 0) begin
		select 6 as ResultCode, 'alter item failed' as ResultMsg
		return
	end
	
	select 0 as ResultCode
	return
	
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_Backpack_SRV_Change]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_Backpack_SRV_Change]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_Backpack_SRV_Change]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_Backpack_SRV_Change]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_Backpack_SRV_Change]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_Backpack_SRV_Change]
	@in_CustomerID int,
	@in_CharID int,
	@in_BackpackID int,
	@in_BackpackSize int
AS
BEGIN
	SET NOCOUNT ON;

	--
	-- _SRV_ function - no validity checks
	--
	
	-- replace backpack size/id
	update UsersChars set BackpackID=@in_BackpackID, BackpackSize=@in_BackpackSize where CharID=@in_CharID
	
	select 0 as ResultCode
	select 0 as 'InventoryID'
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_Backpack_SRV_DeleteItem]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_Backpack_SRV_DeleteItem]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_Backpack_SRV_DeleteItem]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_Backpack_SRV_DeleteItem]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_Backpack_SRV_DeleteItem]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_Backpack_SRV_DeleteItem] 
	@in_CustomerID int,
	@in_CharID int,
	@in_Slot int,
	@in_ItemID int = 0, -- not used
	@in_Amount int = 0, -- not used
	@in_Var1 int = 0, -- not used
	@in_Var2 int = 0 -- not used
AS
BEGIN
	SET NOCOUNT ON;

	delete from UsersInventory where CharID=@in_CharID and BackpackSlot=@in_Slot
	if(@@ROWCOUNT = 0) begin
		select 6 as ResultCode, 'delete item failed' as ResultMsg
		return
	end
	
	select 0 as ResultCode
	return
	
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BackpackChange]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BackpackChange]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_BackpackChange]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BackpackChange]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_BackpackChange]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_BackpackChange]
	@in_CustomerID int,
	@in_CharID int,
	@in_InventoryID bigint
AS
BEGIN
	SET NOCOUNT ON;

	-- check if CustomerID/CharID pair is valid
	declare @CustomerID int
	declare @BackpackSize int
	declare @GameFlags int
	select @CustomerID=CustomerID, @BackpackSize=BackpackSize, @GameFlags=GameFlags FROM UsersChars WHERE CharID=@in_CharID
	if(@@ROWCOUNT = 0 or @CustomerID <> @in_CustomerID) begin
		select 6 as ResultCode, 'bad charid' as ResultMsg
		return
	end
	
	-- do not allow operations outside safe zone
	if((@GameFlags & 1) = 0) begin
		select 9 as ResultCode, 'outside safe zone' as ResultMsg
		return
	end
	
	-- check if game is still active or 90sec passed from last update (COPYPASTE_GAMECHECK, search for others)
	--declare @lastgamedate datetime
	--declare @GameServerId int
	--select @GameServerId=GameServerId, @lastgamedate=lastgamedate from UsersData where CustomerID=@in_CustomerID
	--if(@GameServerId > 0 and DATEDIFF(second, @lastgamedate, GETDATE()) < 90) begin
	--	select 7 as ResultCode, 'game still active' as ResultMsg
	--	return
	--end
	
	-- validate that we own that item
	declare @InvItemID int = 0
	declare @InvCustomerID int = 0
	declare @InvCharID int = 0
	declare @InvQuantity int = 0
	select @InvItemID=ItemID, @InvQuantity=Quantity, @InvCustomerID=CustomerID from UsersInventory where InventoryID=@in_InventoryID
	if(@InvCustomerID <> @in_CustomerID) begin
		select 6 as ResultCode, 'bad inventoryid #1' as ResultMsg
		return
	end
	if(@InvCharID > 0 and @InvCharID <> @in_CharID) begin
		select 6 as ResultCode, 'bad inventoryid #2' as ResultMsg
		return
	end

	-- validate that this is actually a backpack
	declare @MaxSlots int = 0
	select @MaxSlots=Bulkiness from Items_Gear where ItemID=@InvItemID and Category=12
	if(@MaxSlots = 0) begin
		select 6 as ResultCode, 'no backpack' as ResultMsg
		return
	end
	
	-- move everything above current slots to inventory
	update UsersInventory set CharID=0, BackpackSlot=-1 where CharID=@in_CharID and BackpackSlot>=@MaxSlots

	-- remove single backpack from inventory
	set @InvQuantity = @InvQuantity - 1
	if(@InvQuantity <= 0) begin
		delete from UsersInventory where InventoryID=@in_InventoryID
	end 
	else begin
		update UsersInventory set Quantity=@InvQuantity where InventoryID=@in_InventoryID
	end
	
	-- place old backpack to inventory
	declare @OldBackpackID int
	select @OldBackpackID=BackpackID from UsersChars where CharID=@in_CharID
	exec dbo.FN_AddItemToUser @CustomerID, @OldBackpackID, 2000
	
	-- replace backpack size/id
	update UsersChars set BackpackID=@InvItemID, BackpackSize=@MaxSlots where CharID=@in_CharID
	
	select 0 as ResultCode
	select 0 as 'InventoryID'
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BackpackFromInv]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BackpackFromInv]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_BackpackFromInv]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BackpackFromInv]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_BackpackFromInv]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_BackpackFromInv] 
	@in_CustomerID int,
	@in_CharID int,
	@in_InventoryID bigint,
	@in_Slot int,
	@in_Amount int
AS
BEGIN
	SET NOCOUNT ON;
	
	if(@in_Amount <= 0) begin
		select 6 as ResultCode, 'bad amount' as ResultMsg
		return
	end

	-- check if CustomerID/CharID pair is valid
	declare @CustomerID int
	declare @BackpackSize int
	select @CustomerID=CustomerID, @BackpackSize=BackpackSize FROM UsersChars WHERE CharID=@in_CharID
	if(@@ROWCOUNT = 0 or @CustomerID <> @in_CustomerID) begin
		select 6 as ResultCode, 'bad charid' as ResultMsg
		return
	end
	
	-- check if game is still active or 90sec passed from last update (COPYPASTE_GAMECHECK, search for others)
	--declare @lastgamedate datetime
	--declare @GameServerId int
	--select @GameServerId=GameServerId, @lastgamedate=lastgamedate from UsersData where CustomerID=@in_CustomerID
	--if(@GameServerId > 0 and DATEDIFF(second, @lastgamedate, GETDATE()) < 90) begin
	--	select 7 as ResultCode, 'game still active' as ResultMsg
	--	return
	--end
	
	-- check if we have that item in inventory
	declare @InvCustomerID int
	declare @InvInventoryID bigint
	declare @InvItemID int
	declare @InvLeasedUntil datetime
	declare @InvQuantity int
	declare @InvVar1 int
	declare @InvVar2 int
	select 
		@InvCustomerID=CustomerID,
		@InvInventoryID=InventoryID,
		@InvItemID=ItemID, 
		@InvQuantity=Quantity,
		@InvLeasedUntil=LeasedUntil,
		@InvVar1=Var1,
		@InvVar2=Var2
	from UsersInventory where InventoryID=@in_InventoryID
	if(@@ROWCOUNT = 0 or @InvCustomerID <> @in_CustomerID) begin
		select 6 as ResultCode, 'bad inventoryid' as ResultMsg
		return
	end
	
	if(@in_Amount > @InvQuantity) begin
		select 6 as ResultCode, 'bad quantity' as ResultMsg
		return
	end
	
	-- validate backpack slot number
	if(@in_Slot < 0 or @in_Slot >= @BackpackSize) begin
		select 6 as ResultCode, 'bad slot' as ResultMsg
		return
	end
	-- validate itemid in that slot
	declare @BackpackInventoryID bigint = 0
	declare @BackpackItemID int = 0
	select 
		@BackpackInventoryID=InventoryID,
		@BackpackItemID=ItemID
	from UsersInventory where CharID=@in_CharID and BackpackSlot=@in_Slot
	if(@@ROWCOUNT > 0 and @BackpackItemID <> @InvItemID) begin
		select 6 as ResultCode, 'slot itemid mismatch' as ResultMsg
		return
	end

	-- clear attachments if moved item was in weapon slot
	if(@in_Slot = 0) update UsersChars set Attachment1='' where CharID=@in_CharID
	if(@in_Slot = 1) update UsersChars set Attachment2='' where CharID=@in_CharID

	-- check for easy case, unmodified item, no such item in backpack
	if(@BackpackInventoryID = 0 and @InvQuantity = @in_Amount) begin
		update UsersInventory set BackpackSlot=@in_Slot, CharID=@in_CharID where InventoryID=@InvInventoryID

		select 0 as ResultCode
		select @InvInventoryID as 'InventoryID'
		return
	end

	if(@BackpackInventoryID = 0)
	begin
		-- modified (won't stack) or new backpack item
		INSERT INTO UsersInventory (
			CustomerID,
			CharID,
			ItemID,
			BackpackSlot,
			LeasedUntil, 
			Quantity,
			Var1,
			Var2
		)
		VALUES (
			@in_CustomerID,
			@in_CharID,
			@InvItemID,
			@in_Slot,
			@InvLeasedUntil,
			@in_Amount,
			@InvVar1,
			@InvVar2
		)
		set @BackpackInventoryID = SCOPE_IDENTITY()
	end
	else
	begin
		update UsersInventory set Quantity=(Quantity+@in_Amount) where InventoryID=@BackpackInventoryID
	end
	
	-- from inventory
	set @InvQuantity = @InvQuantity - @in_Amount
	if(@InvQuantity <= 0) begin
		delete from UsersInventory where InventoryID=@InvInventoryID
	end 
	else begin
		update UsersInventory set Quantity=@InvQuantity where InventoryID=@InvInventoryID
	end
	
	select 0 as ResultCode;
	select @BackpackInventoryID as 'InventoryID'
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BackpackGridJoin]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BackpackGridJoin]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_BackpackGridJoin]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BackpackGridJoin]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_BackpackGridJoin]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_BackpackGridJoin]
	@in_CustomerID int,
	@in_CharID int,
	@in_SlotFrom int,
	@in_SlotTo int
AS
BEGIN
	SET NOCOUNT ON;

	-- check if CustomerID/CharID pair is valid
	declare @CustomerID int
	declare @BackpackSize int
	select @CustomerID=CustomerID, @BackpackSize=BackpackSize FROM UsersChars WHERE CharID=@in_CharID
	if(@@ROWCOUNT = 0 or @CustomerID <> @in_CustomerID) begin
		select 6 as ResultCode, 'bad charid' as ResultMsg
		return
	end

	-- check if game is still active or 90sec passed from last update (COPYPASTE_GAMECHECK, search for others)
	--declare @lastgamedate datetime
	--declare @GameServerId int
	--select @GameServerId=GameServerId, @lastgamedate=lastgamedate from UsersData where CustomerID=@in_CustomerID
	--if(@GameServerId > 0 and DATEDIFF(second, @lastgamedate, GETDATE()) < 90) begin
	--	select 7 as ResultCode, 'game still active' as ResultMsg
	--	return
	--end
	
	-- check from slot
	declare @FromItemID int
	declare @FromQuantity int
	declare @FromVar1 int
	declare @FromVar2 int
	select 
		@FromItemID=ItemID, 
		@FromQuantity=Quantity,
		@FromVar1=Var1,
		@FromVar2=Var2
	from UsersInventory where CharID=@in_CharID and BackpackSlot=@in_SlotFrom
	if(@@ROWCOUNT = 0 or @FromItemID = 0) begin
		select 6 as ResultCode, 'bad slot1' as ResultMsg
		return
	end
	
	-- check to slot
	declare @ToItemID int
	declare @ToQuantity int
	declare @ToVar1 int
	declare @ToVar2 int
	select 
		@ToItemID=ItemID, 
		@ToQuantity=Quantity,
		@ToVar1=Var1,
		@ToVar2=Var2
	from UsersInventory where CharID=@in_CharID and BackpackSlot=@in_SlotTo
	if(@@ROWCOUNT = 0 or @ToItemID = 0) begin
		select 6 as ResultCode, 'bad slot2' as ResultMsg
		return
	end
	
	if(@ToItemID <> @FromItemID or @FromVar1 >= 0 or @ToVar1 >= 0) begin
		select 6 as ResultCode, 'bad join' as ResultMsg
		return
	end
	
	-- swap slots. operation will silently be ok if there is no item in that slot
	update UsersInventory set Quantity=Quantity+@FromQuantity where CharID=@in_CharID and BackpackSlot=@in_SlotTo
	delete from UsersInventory where CharID=@in_CharID and BackpackSlot=@in_SlotFrom
	
	select 0 as ResultCode
	select 0 as 'InventoryID'
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BackpackGridSwap]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BackpackGridSwap]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_BackpackGridSwap]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BackpackGridSwap]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_BackpackGridSwap]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_BackpackGridSwap] 
	@in_CustomerID int,
	@in_CharID int,
	@in_SlotFrom int,
	@in_SlotTo int
AS
BEGIN
	SET NOCOUNT ON;

	-- check if CustomerID/CharID pair is valid
	declare @CustomerID int
	declare @BackpackSize int
	select @CustomerID=CustomerID, @BackpackSize=BackpackSize FROM UsersChars WHERE CharID=@in_CharID
	if(@@ROWCOUNT = 0 or @CustomerID <> @in_CustomerID) begin
		select 6 as ResultCode, 'bad charid' as ResultMsg
		return
	end
	
	-- check if game is still active or 90sec passed from last update (COPYPASTE_GAMECHECK, search for others)
	--declare @lastgamedate datetime
	--declare @GameServerId int
	--select @GameServerId=GameServerId, @lastgamedate=lastgamedate from UsersData where CustomerID=@in_CustomerID
	--if(@GameServerId > 0 and DATEDIFF(second, @lastgamedate, GETDATE()) < 90) begin
	--	select 7 as ResultCode, 'game still active' as ResultMsg
	--	return
	--end
	
	-- validate backpack slot number
	if(@in_SlotFrom < 0 or @in_SlotFrom >= @BackpackSize) begin
		select 6 as ResultCode, 'bad slot' as ResultMsg
		return
	end
	if(@in_SlotTo < 0 or @in_SlotTo >= @BackpackSize) begin
		select 6 as ResultCode, 'bad slot' as ResultMsg
		return
	end
	
	-- get inventory ids of both slots
	declare @InventoryIdFrom bigint = 0
	declare @InventoryIdTo bigint = 0
	select @InventoryIdFrom=InventoryID from UsersInventory where CharID=@in_CharID and BackpackSlot=@in_SlotFrom
	select @InventoryIdTo=InventoryID   from UsersInventory where CharID=@in_CharID and BackpackSlot=@in_SlotTo

	-- swap slots. operation will silently be ok if there is no item in that slot
	update UsersInventory set BackpackSlot=@in_SlotTo   where InventoryID=@InventoryIdFrom
	update UsersInventory set BackpackSlot=@in_SlotFrom where InventoryID=@InventoryIdTo
	
	-- clear attachments if swapped items was in weapon slots
	if(@in_SlotTo = 0 or @in_SlotFrom = 0) update UsersChars set Attachment1='' where CharID=@in_CharID
	if(@in_SlotTo = 1 or @in_SlotFrom = 1) update UsersChars set Attachment2='' where CharID=@in_CharID
	
	select 0 as ResultCode
	select 0 as 'InventoryID'
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BackpackToInv]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BackpackToInv]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_BackpackToInv]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BackpackToInv]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_BackpackToInv]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_BackpackToInv] 
	@in_CustomerID int,
	@in_CharID int,
	@in_InventoryID bigint,	-- target inventory id where to put that item
	@in_Slot int,
	@in_Amount int
AS
BEGIN
	SET NOCOUNT ON;

	if(@in_Amount <= 0) begin
		select 6 as ResultCode, 'bad amount' as ResultMsg
		return
	end

	-- check if CustomerID/CharID pair is valid
	declare @CustomerID int
	select @CustomerID=CustomerID FROM UsersChars WHERE CharID=@in_CharID
	if(@@ROWCOUNT = 0 or @CustomerID <> @in_CustomerID) begin
		select 6 as ResultCode, 'bad charid' as ResultMsg
		return
	end
	
	-- check if game is still active or 90sec passed from last update (COPYPASTE_GAMECHECK, search for others)
	--declare @lastgamedate datetime
	--declare @GameServerId int
	--select @GameServerId=GameServerId, @lastgamedate=lastgamedate from UsersData where CustomerID=@in_CustomerID
	--if(@GameServerId > 0 and DATEDIFF(second, @lastgamedate, GETDATE()) < 90) begin
	--	select 7 as ResultCode, 'game still active' as ResultMsg
	--	return
	--end
	
	declare @BackpackInventoryID bigint
	declare @BackpackItemID int
	declare @BackpackLeasedUntil datetime
	declare @BackpackQuantity int
	declare @BackpackVar1 int
	declare @BackpackVar2 int
	select 
		@BackpackInventoryID=InventoryID,
		@BackpackItemID=ItemID, 
		@BackpackQuantity=Quantity,
		@BackpackLeasedUntil=LeasedUntil,
		@BackpackVar1=Var1,
		@BackpackVar2=Var2
	from UsersInventory where CharID=@in_CharID and BackpackSlot=@in_Slot
	if(@@ROWCOUNT = 0) begin
		select 6 as ResultCode, 'bad slot' as ResultMsg
		return
	end
	
	if(@in_Amount > @BackpackQuantity) begin
		select 6 as ResultCode, 'bad quantity' as ResultMsg
		return
	end
	
	-- check for easy case, just switching to inventory
	if(@in_InventoryID = 0 and @BackpackQuantity = @in_Amount) begin
		update UsersInventory set BackpackSlot=-1, CharID=0 where InventoryID=@BackpackInventoryID

		select 0 as ResultCode
		select @BackpackInventoryID as 'InventoryID'
		return
	end
	
	-- validate that we own that inventory slot and item can be moved there
	if(@in_InventoryID > 0) 
	begin
		declare @InvCustomerID int
		declare @InvCharID int
		declare @InvItemID int
		declare @InvVar1 int
		select 
			@InvCustomerID=CustomerID,
			@InvCharID=CharID, 
			@InvItemID=ItemID, 
			@InvVar1=Var1 
			from UsersInventory where InventoryID=@in_InventoryID
		if(@@ROWCOUNT = 0 or @InvCustomerID <> @in_CustomerID or @InvCharID <> 0 or @InvItemID <> @BackpackItemID) begin
			select 6 as ResultCode, 'bad inventoryid' as ResultMsg
			return
		end
		if(@InvVar1 >= 0 or @BackpackVar1 >= 0) begin
			select 6 as ResultCode, 'not stackable' as ResultMsg
			return
		end
	end

	declare @InvInventoryID bigint = @in_InventoryID
	if(@InvInventoryID = 0) begin
		-- modified (won't stack) or new inventory item
		INSERT INTO UsersInventory (
			CustomerID,
			CharID,
			ItemID, 
			LeasedUntil, 
			Quantity,
			Var1,
			Var2
		)
		VALUES (
			@in_CustomerID,
			0,
			@BackpackItemID,
			@BackpackLeasedUntil,
			@in_Amount,
			@BackpackVar1,
			@BackpackVar2
		)
		set @InvInventoryID = SCOPE_IDENTITY()
	end
	else begin
		update UsersInventory set Quantity=(Quantity+@in_Amount) where InventoryID=@InvInventoryID
	end

	-- from backpack
	set @BackpackQuantity = @BackpackQuantity - @in_Amount
	if(@BackpackQuantity <= 0) begin
		delete from UsersInventory where InventoryID=@BackpackInventoryID
	end 
	else begin
		update UsersInventory set Quantity=@BackpackQuantity where InventoryID=@BackpackInventoryID
	end
	
	select 0 as ResultCode
	select @InvInventoryID as 'InventoryID'
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BuyItem_GD]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BuyItem_GD]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_BuyItem_GD]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BuyItem_GD]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_BuyItem_GD]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_BuyItem_GD]
	@in_IP char(32),
	@in_CustomerID int,
	@in_ItemId int,
	@in_BuyDays int
AS
BEGIN
	SET NOCOUNT ON;

	-- get points for that customer
	declare @GameDollars int
	SELECT @GameDollars=GameDollars FROM UsersData WHERE CustomerID=@in_CustomerID
	if (@@RowCount = 0) begin
		select 6 as ResultCode, 'no CustomerID' as ResultMsg
		return
	end

	declare @smsg1 varchar(1000)
	declare @out_FNResult int = 100

	-- get price
	declare @FinalPrice int
	exec WZ_BuyItemFN_GetPrice @out_FNResult out, @in_ItemId, @in_BuyDays, 'GD', @FinalPrice out
	if(@out_FNResult > 0) begin
		set @smsg1 = LTRIM(STR(@out_FNResult)) + ' GD '
		set @smsg1 = @smsg1 + LTRIM(STR(@in_BuyDays)) + ' ' + LTRIM(STR(@in_ItemID))
		EXEC FN_ADD_SECURITY_LOG 110, @in_IP, @in_CustomerID, @smsg1
		select 6 as ResultCode, 'bad GetPrice' as ResultMsg
		return
	end
	
	-- check if enough money
	if(@GameDollars < @FinalPrice) begin
		set @smsg1 = LTRIM(STR(@in_ItemId)) + ' ' + LTRIM(STR(@in_BuyDays)) + ' '
		set @smsg1 = @smsg1 + ' GD ' + LTRIM(STR(@FinalPrice)) + ' ' + LTRIM(STR(@GameDollars))
		EXEC FN_ADD_SECURITY_LOG 114, @in_IP, @in_CustomerID, @smsg1
		select 7 as ResultCode, 'Not Enough GD' as ResultMsg
		return
	end

	-- exec item adding function, if it fail, do not process transaction further
	exec WZ_BuyItemFN_Exec @out_FNResult out, @in_CustomerID, @in_ItemId, @in_BuyDays
	if(@out_FNResult <> 0) begin
		set @smsg1 = 'BuyExec failed' + LTRIM(STR(@out_FNResult))
		select 7 as ResultCode, @smsg1 as ResultMsg
		return
	end

	-- perform actual transaction
	set @GameDollars = @GameDollars-@FinalPrice;
	UPDATE UsersData SET GameDollars=@GameDollars where CustomerID=@in_CustomerID

	-- set transaction type
	declare @TType int = 0
	if(@in_BuyDays = 2000) set @TType = 3001;
	else set @TType = 2001;
	
	-- update transaction detail
	INSERT INTO FinancialTransactions
		VALUES (@in_CustomerID, 'INGAME', @TType, GETDATE(), 
				@FinalPrice, '1', 'APPROVED', @in_ItemId)

	-- search for InventoryID of added item
	declare @InventoryID bigint = 0
	select @InventoryID=InventoryID from UsersInventory
		where CustomerID=@in_CustomerID and CharID=0 and ItemID=@in_ItemId and Var1<0

	select 0 as ResultCode
	select @GameDollars as 'Balance', @InventoryID as 'InventoryID'

END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BuyItem_GP]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BuyItem_GP]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_BuyItem_GP]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BuyItem_GP]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_BuyItem_GP]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_BuyItem_GP]
	@in_IP char(32),
	@in_CustomerID int,
	@in_ItemId int,
	@in_BuyDays int
AS
BEGIN
	SET NOCOUNT ON;

	-- get points for that customer
	declare @GamePoints int
	SELECT @GamePoints=GamePoints FROM UsersData WHERE CustomerID=@in_CustomerID
	if (@@RowCount = 0) begin
		select 6 as ResultCode, 'no CustomerID' as ResultMsg
		return
	end

	declare @smsg1 varchar(1000)
	declare @out_FNResult int = 100

	-- get price
	declare @FinalPrice int
	exec WZ_BuyItemFN_GetPrice @out_FNResult out, @in_ItemId, @in_BuyDays, 'GP', @FinalPrice out
	if(@out_FNResult > 0) begin
		set @smsg1 = LTRIM(STR(@out_FNResult)) + ' GP '
		set @smsg1 = @smsg1 + LTRIM(STR(@in_BuyDays)) + ' ' + LTRIM(STR(@in_ItemID))
		EXEC FN_ADD_SECURITY_LOG 110, @in_IP, @in_CustomerID, @smsg1
		select 6 as ResultCode, 'bad GetPrice' as ResultMsg
		return
	end
	
	-- check if enough money
	if(@GamePoints < @FinalPrice) begin
		set @smsg1 = LTRIM(STR(@in_ItemId)) + ' ' + LTRIM(STR(@in_BuyDays)) + ' '
		set @smsg1 = @smsg1 + ' GP ' + LTRIM(STR(@FinalPrice)) + ' ' + LTRIM(STR(@GamePoints))
		EXEC FN_ADD_SECURITY_LOG 114, @in_IP, @in_CustomerID, @smsg1
		select 7 as ResultCode, 'Not Enough GP' as ResultMsg
		return
	end

	-- exec item adding function, if it fail, do not process transaction further
	exec WZ_BuyItemFN_Exec @out_FNResult out, @in_CustomerID, @in_ItemId, @in_BuyDays
	if(@out_FNResult <> 0) begin
		set @smsg1 = 'BuyExec failed' + LTRIM(STR(@out_FNResult))
		select 7 as ResultCode, @smsg1 as ResultMsg
		return
	end

	-- perform actual transaction
	declare @AlterGP int = -@FinalPrice;
	exec FN_AlterUserGP @in_CustomerID, @AlterGP, 'WZ_BuyItem_GP'
	set @GamePoints=@GamePoints-@FinalPrice;

	-- set transaction type
	declare @TType int = 0
	if(@in_BuyDays = 2000) set @TType = 3000;
	else set @TType = 2000;
	
	-- update transaction detail
	INSERT INTO FinancialTransactions
		VALUES (@in_CustomerID, 'INGAME', @TType, GETDATE(), 
				@FinalPrice, '1', 'APPROVED', @in_ItemId)
				
	-- search for InventoryID of added item
	declare @InventoryID bigint = 0
	select @InventoryID=InventoryID from UsersInventory
		where CustomerID=@in_CustomerID and CharID=0 and ItemID=@in_ItemId and Var1<0

	select 0 as ResultCode
	select @GamePoints as 'Balance', @InventoryID as 'InventoryID';

END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BuyItemFN_Exec]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BuyItemFN_Exec]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_BuyItemFN_Exec]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BuyItemFN_Exec]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_BuyItemFN_Exec]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_BuyItemFN_Exec]
	@out_FNResult int out,
	@in_CustomerID int,
	@in_ItemId int,
	@in_BuyDays int
AS
BEGIN
	SET NOCOUNT ON;
	
	--
	--
	-- main function for buying items in game, should be called from WZ_BuyItem2
	--
	--

	-- set success by default
	set @out_FNResult = 0
	
	--SAMPLE ITEM 10k GD
	--if(@in_ItemId = 301107) begin
	--	update UsersData set GameDollars=GameDollars+10000 where CustomerID=@in_CustomerID
	--	return
	--end
	
	-- clan items. NOTE: no item adding
	if(@in_ItemId >= 301151 and @in_ItemId <= 301157) begin
		return
	end
	
	-- normal item
	exec FN_AddItemToUser @in_CustomerID, @in_ItemId, @in_BuyDays
	set @out_FNResult = 0

END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BuyItemFN_GetPrice]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BuyItemFN_GetPrice]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_BuyItemFN_GetPrice]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_BuyItemFN_GetPrice]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_BuyItemFN_GetPrice]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_BuyItemFN_GetPrice]
    @out_FNResult int out,
	@in_ItemId int,
	@in_BuyDays int,
	@in_Currency varchar(32),
	@o_FinalPrice int out
AS
BEGIN
	SET NOCOUNT ON;
	
--
-- get prices from table based on itemID
--
	declare @Price1 int = 0
	declare @Price7 int = 0
	declare @Price30 int = 0
	declare @PriceP int = 0
	declare @GPrice1 int = 0
	declare @GPrice7 int = 0
	declare @GPrice30 int = 0
	declare @GPriceP int = 0
	declare @IsEnabled int = 1

	if(@in_ItemId >= 20000 and @in_ItemId < 99999)
		SELECT
		   @Price1=Price1, @Price7=Price7, @Price30=Price30, @PriceP=PriceP, 
		   @GPrice1=GPrice1, @GPrice7=GPrice7, @GPrice30=GPrice30, @GPriceP=GPriceP
		FROM Items_Gear where ItemID=@in_ItemID
	else
	if(@in_ItemId >= 100000 and @in_ItemId < 190000)
		SELECT
		   @Price1=Price1, @Price7=Price7, @Price30=Price30, @PriceP=PriceP, 
		   @GPrice1=GPrice1, @GPrice7=GPrice7, @GPrice30=GPrice30, @GPriceP=GPriceP
		FROM Items_Weapons where ItemID=@in_ItemID
	else 
	if(@in_ItemId >= 300000 and @in_ItemId < 390000) 
		SELECT
		   @Price1=Price1, @Price7=Price7, @Price30=Price30, @PriceP=PriceP, 
		   @GPrice1=GPrice1, @GPrice7=GPrice7, @GPrice30=GPrice30, @GPriceP=GPriceP
		FROM Items_Generic where ItemID=@in_ItemID
	else 
	if(@in_ItemId >= 400000 and @in_ItemId < 490000) 
		SELECT
		   @Price1=Price1, @Price7=Price7, @Price30=Price30, @PriceP=PriceP, 
		   @GPrice1=GPrice1, @GPrice7=GPrice7, @GPrice30=GPrice30, @GPriceP=GPriceP
		FROM Items_Attachments where ItemID=@in_ItemID
	else 
	begin
		set @out_FNResult = 1
		return
	end
	if (@@RowCount = 0) begin
		set @out_FNResult = 2
		return
	end
	
	     if(@in_Currency = 'GP' and @in_BuyDays = 1)    set @o_FinalPrice = @Price1
	else if(@in_Currency = 'GP' and @in_BuyDays = 7)    set @o_FinalPrice = @Price7
	else if(@in_Currency = 'GP' and @in_BuyDays = 30)   set @o_FinalPrice = @Price30
	else if(@in_Currency = 'GP' and @in_BuyDays = 2000) set @o_FinalPrice = @PriceP
	else if(@in_Currency = 'GD' and @in_BuyDays = 1)    set @o_FinalPrice = @GPrice1
	else if(@in_Currency = 'GD' and @in_BuyDays = 7)    set @o_FinalPrice = @GPrice7
	else if(@in_Currency = 'GD' and @in_BuyDays = 30)   set @o_FinalPrice = @GPrice30
	else if(@in_Currency = 'GD' and @in_BuyDays = 2000) set @o_FinalPrice = @GPriceP
	else begin
		set @out_FNResult = 3
		return
	end

	-- check if listed
	if(@o_FinalPrice <= 0 or @IsEnabled = 0) begin
		set @out_FNResult = 4
		return
	end
	
	set @out_FNResult = 0
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_Char_SRV_SetAttachments]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_Char_SRV_SetAttachments]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_Char_SRV_SetAttachments]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_Char_SRV_SetAttachments]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_Char_SRV_SetAttachments]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_Char_SRV_SetAttachments]
	@in_CustomerID int,
	@in_CharID int,
	@in_Attm1 varchar(256),
	@in_Attm2 varchar(256)
AS
BEGIN
	SET NOCOUNT ON;
	
	--
	-- this function should be called only by server, so we skip all validations
	--

	-- update attachments
	UPDATE UsersChars SET Attachment1=@in_Attm1, Attachment2=@in_Attm2 where CharID=@in_CharID

	select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_Char_SRV_SetStatus]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_Char_SRV_SetStatus]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_Char_SRV_SetStatus]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_Char_SRV_SetStatus]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_Char_SRV_SetStatus]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_Char_SRV_SetStatus]
	@in_CustomerID int,
	@in_CharID int,
	@in_Alive int,
	@in_GamePos varchar(256),
	@in_GameFlags int,
	@in_Health float,
	@in_Hunger float,
	@in_Thirst float,
	@in_Toxic float,
	@in_TimePlayed int,
	@in_XP int,
	@in_Reputation int,
	@in_GameDollars int,
	@in_Stat00 int,
	@in_Stat01 int,
	@in_Stat02 int,
	@in_Stat03 int,
	@in_Stat04 int,
	@in_Stat05 int,
	@in_GroupID int,
	@in_legfall int,
	@in_bleeding int,
	@in_Mission1 int
AS
BEGIN
	SET NOCOUNT ON;
	
	--
	-- this function should be called only by server, so we skip all validations
	--
	
	-- record last game update
	update UsersData set GameDollars=@in_GameDollars, lastgamedate=GETDATE() where CustomerID=@in_CustomerID

	-- update basic character data
	update UsersChars set
		GamePos=@in_GamePos,
		GameFlags=@in_GameFlags,
		Alive=@in_Alive,
		Health=@in_Health,
		Food=@in_Hunger,
		Water=@in_Thirst,
		Toxic=@in_Toxic,
		TimePlayed=@in_TimePlayed,
		LastUpdateDate=GETDATE(),
		XP=@in_XP,
		Reputation=@in_Reputation,
		Stat00=@in_Stat00,
		Stat01=@in_Stat01,
		Stat02=@in_Stat02,
		Stat03=@in_Stat03,
		Stat04=@in_Stat04,
		Stat05=@in_Stat05,
		GroupID=@in_GroupID,
		bleeding=@in_bleeding,
		legfall=@in_legfall,
		Mission1=@in_Mission1

	where CharID=@in_CharID
	
	if(@in_Alive = 0) begin
		update UsersChars set DeathUtcTime=GETUTCDATE() where CharID=@in_CharID
		-- update of backpacks
        update UsersChars set BackpackID=20176, BackpackSize=12 where CharID=@in_CharID 
		-- delete stuff from backpack
		--delete from UsersInventory where CustomerID=@in_CustomerID and CharID=@in_CharID
		delete from UsersInventory where CharID=@in_CharID
	end
	select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_CharChangeName]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_CharChangeName]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_CharChangeName]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_CharChangeName]
-- ----------------------------
CREATE PROCEDURE [dbo].[WZ_CharChangeName]
	@in_CustomerID int,
	@in_CharID int,
	@in_Gamertag varchar(128)
AS
BEGIN
	SET NOCOUNT ON;
	
	declare @GamePoints int
	declare @CustomerID int = 0
	declare @GamerTag NVarchar(64)
	
	-- validate CharID/CustomerID pair
	select @CustomerID=CustomerID, @GamerTag=Gamertag from UsersChars where CharID=@in_CharID
	if(@@ROWCOUNT = 0 or @CustomerID <> @in_CustomerID) begin
		select 6 as ResultCode, 'bad charid' as ResultMsg
		return
	end
	
	-- Check for GC
	SELECT @GamePoints=GamePoints FROM UsersData WHERE CustomerID=@CustomerID
	If(@GamePoints < 80) 
	Begin
		select 7 as ResultCode, 'Not Enough GP' as ResultMsg
		return
	End

	-- Validate new GamerTag
	IF exists (select CharID from UsersChars where Gamertag=@in_Gamertag)
	Begin
		select 9 as ResultCode, 'Gamertag already exists' as ResultMsg
		return
	End
	
	--Save to Log
	INSERT INTO CharsLog (CharMessage, CustomerID, CharID, GamerTag, time) VALUES ('Char name change to '+@in_Gamertag, @CustomerID, @in_CharID, @GamerTag, GETDATE())
	
	-- Update GamerTag and GC
	update UsersData set GamePoints=(GamePoints-80) WHERE CustomerID=@CustomerID
	update UsersChars set Gamertag=@in_Gamertag where CharID=@in_CharID

select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_CharChangeOutfit]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_CharChangeOutfit]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_CharChangeOutfit]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_CharChangeOutfit]
-- ----------------------------
CREATE PROCEDURE [dbo].[WZ_CharChangeOutfit]
    @in_CustomerID int,
    @in_CharID int,
    @in_HeadIdx int,
    @in_BodyIdx int,
    @in_LegsIdx int
AS
BEGIN
    SET NOCOUNT ON;
    
    if(@in_CharID = 0)
        return

    update UsersChars set HeadIdx=@in_HeadIdx,BodyIdx=@in_BodyIdx,LegsIdx=@in_LegsIdx where CharID=@in_CharID
	select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_CharCreate]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_CharCreate]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_CharCreate]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_CharCreate]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_CharCreate]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_CharCreate] 
	@in_CustomerID int,
	@in_Hardcore int,
	@in_Gamertag nvarchar(64),
	@in_HeroItemID int,
	@in_HeadIdx int,
	@in_BodyIdx int,
	@in_LegsIdx int
AS
BEGIN
	SET NOCOUNT ON;

	if(@in_Gamertag like '%sergey%titov%') begin
		select 9 as ResultCode, 'no impersonation' as ResultMsg
		return
	end

	if(@in_Gamertag like '%titov%sergey%') begin
		select 9 as ResultCode, 'no impersonation' as ResultMsg
		return
	end

	if(@in_Gamertag like '%system%') begin
		select 9 as ResultCode, 'no System' as ResultMsg
		return
	end
	
	if(@in_Gamertag like '%\[dev\]%' escape '\') begin
		select 9 as ResultCode, 'no dev' as ResultMsg
		return
	end
	
	-- check if gamertag is unique
	if exists (select CharID from UsersChars where Gamertag=@in_Gamertag)
	begin
		select 9 as ResultCode, 'Gamertag already exists' as ResultMsg
		return
	end

-- for anticheats we will used this to check heroitemid is vaild.
  declare @cat int = 0
	select @cat=Category from Items_Gear where ItemID=@in_HeroItemID
   if (@cat != 16) BEGIN
	  select 6 as ResultCode, 'Invaid HeroItemID' as ResultMsg
		return
	end

-- anticheats : check this user have Hero.

	-- we can't have more that 5 survivors
	declare @NumChars int = 0
	select @NumChars=COUNT(*) from UsersChars where CustomerID=@in_CustomerID
	if(@NumChars >= 5) begin
		select 6 as ResultCode, 'too many created chars' as ResultMsg
		return
	end

	insert into UsersChars (
		CustomerID,
		Gamertag,
		Alive,
		Hardcore,
		HeroItemID,
		HeadIdx,
		BodyIdx,
		LegsIdx,
		CreateDate
	) values (
		@in_CustomerID,
		@in_Gamertag,
		3,
		@in_Hardcore,
		@in_HeroItemID,
		@in_HeadIdx,
		@in_BodyIdx,
		@in_LegsIdx,
		GETDATE()
	)
	declare @CharID int = SCOPE_IDENTITY()
	
	-- give basic items for first few survivors
	declare @CharsCreated int = 0
	update UsersData set CharsCreated=(CharsCreated+1) where CustomerID=@in_CustomerID
	select @CharsCreated=CharsCreated from UsersData where CustomerID=@in_CustomerID
	if(@CharsCreated <= 5) begin
		-- add some default items - BE ULTRA CAREFUL with BackpackSlot number
		insert into UsersInventory (CustomerID, CharID, BackpackSlot, ItemID, LeasedUntil, Quantity)
			values (@in_CustomerID, @CharID, 1, 101306, '2020-1-1', 1) -- Flashlight
		insert into UsersInventory (CustomerID, CharID, BackpackSlot, ItemID, LeasedUntil, Quantity)
			values (@in_CustomerID, @CharID, 2, 101261, '2020-1-1', 1) -- Bandages
		insert into UsersInventory (CustomerID, CharID, BackpackSlot, ItemID, LeasedUntil, Quantity)
			values (@in_CustomerID, @CharID, 3, 101296, '2020-1-1', 1) -- Can of Soda
		insert into UsersInventory (CustomerID, CharID, BackpackSlot, ItemID, LeasedUntil, Quantity)
			values (@in_CustomerID, @CharID, 4, 101289, '2020-1-1', 1) -- Granola Bar
	end
	
	-- Krit give 500XP if first char
	if(@CharsCreated = 1) begin
		UPDATE UsersChars SET XP = 500 WHERE CharID = @CharID
	end
	
	-- allow to use postbox on newly created survivors
	update UsersChars set GameFlags=1 where CharID=@CharID
	
	-- Save to CharsCreateLog
	--INSERT INTO CharsCreateLog (CustomerID, CharID, GamerTag) VALUES (@in_CustomerID, @CharID, @in_Gamertag)
	INSERT INTO CharsLog (CharMessage, CustomerID, CharID, GamerTag, time) VALUES ('Char Create', @in_CustomerID, @CharID, @in_Gamertag, GETDATE())
	
	select 0 as ResultCode
	select @CharID as 'CharID'
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_CharDelete]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_CharDelete]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_CharDelete]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_CharDelete]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_CharDelete]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_CharDelete] 
	@in_CustomerID int,
	@in_CharID int
AS
BEGIN
	SET NOCOUNT ON;
	
	-- validate CharID/CustomerID pair
	declare @CustomerID int = 0
	declare @GamerTag NVarchar(64)
	
	select @CustomerID=CustomerID, @GamerTag=Gamertag from UsersChars where CharID=@in_CharID
	if(@@ROWCOUNT = 0 or @CustomerID <> @in_CustomerID) begin
		select 6 as ResultCode, 'bad charid' as ResultMsg
		return
	end

	-- remove from clan
	declare @ClanID int = 0
	declare @ClanRank int = 0
	select @ClanID=ClanID, @ClanRank=ClanRank from UsersChars where CharID=@in_CharID
	if(@ClanID > 0)
	begin
		-- if leader is leaving
		if(@ClanRank = 0)
		begin
			-- check if there is people left in clan
			declare @NumClanMembers int
			select @NumClanMembers=COUNT(*) from UsersChars where ClanID=@ClanID
			if(@NumClanMembers > 1) begin
				select 7 as ResultCode, 'still members in clan'
				return
			end

			-- delete clan
			exec WZ_ClanFN_DeleteClan @ClanID
		end
		else
		begin
			-- not leader, just leave clan
			update ClanData set NumClanMembers=(NumClanMembers - 1) where ClanID=@ClanID
		end
	end
	
	--Save to Log
	INSERT INTO CharsLog (CharMessage, CustomerID, CharID, GamerTag, time) VALUES ('Char Delete', @CustomerID, @in_CharID, @GamerTag, GETDATE())
	
	delete from UsersChars where CharID=@in_CharID
	delete from UsersInventory where CharID=@in_CharID

	-- success
	select 0 as ResultCode
	
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_CharRevive]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_CharRevive]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [WZ_CharRevive]
-- ----------------------------


CREATE PROCEDURE [dbo].[WZ_CharRevive]
    @in_CustomerID int,
    @in_CharID int,
    @in_Health int
AS
BEGIN
    SET NOCOUNT ON;
    
    -- validate CharID/CustomerID pair
    declare @CustomerID int = 0
    select @CustomerID=CustomerID from UsersChars where CharID=@in_CharID
    if(@@ROWCOUNT = 0 or @CustomerID <> @in_CustomerID) begin
        select 6 as ResultCode, 'bad charid' as ResultMsg
        return
    end
    
    -- get developer flag
    declare @IsDeveloper int = 0
    select @IsDeveloper=IsDeveloper from UsersData where CustomerID=@in_CustomerID


    -- note that revive timer is 1hrs, change in WZ_GetAccountInfo1 as well
    declare @sectorevive int
    declare @alive int = 0
    select 
     @sectorevive=DATEDIFF (second, GETUTCDATE (), DATEADD (second, 600, DeathUtcTime)),
     @alive=Alive
    from UsersChars where CharID=@in_CharID


    -- prevent fast teleporting if we're not dead
    if (@alive <> 0) begin
        select 6 as ResultCode, 'character is not dead' as ResultMsg
        return
    end
    
    -- do not allow early revive, give 2min grace


    if (@sectorevive < 30) begin
            select 0 as ResultCode
            update UsersChars set
        Alive=2,
        Health=@in_Health, -- AomBESkillSystem : Health value from api.
        Food=0,
        Water=0,
        Toxic=0,
        GameFlags=1
    where CharID=@in_CharID
        return
    end
        else
         declare @GamePoints int
    SELECT @GamePoints=GamePoints FROM UsersData WHERE CustomerID=@in_CustomerID

    if (@GamePoints < 25) begin
        select 7 as ResultCode, 'Not Enough GP' as ResultMsg
        return
    end

		if (@GamePoints >= 25) begin
    update UsersData set GamePoints=(GamePoints-25) WHERE CustomerID=@in_CustomerID
		end
    select 0 as ResultCode
    
    -- revive
    update UsersChars set
        Alive=2,
        Health=@in_Health, -- AomBESkillSystem : Health value from api.
        Food=0,
        Water=0,
        Toxic=0,
        GameFlags=1,
        GroupID=0
    where CharID=@in_CharID


    select 0 as ResultCode
END



GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanAddClanMembers]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanAddClanMembers]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanAddClanMembers]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanAddClanMembers]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ClanAddClanMembers]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ClanAddClanMembers]
	@in_CharID int,
	@in_ItemID int
AS
BEGIN
	SET NOCOUNT ON;
	
	-- clan id valudation of caller
	declare @ClanID int = 0
	declare @ClanRank int
	declare @Gamertag nvarchar(64)
	select @ClanID=ClanID, @ClanRank=ClanRank, @Gamertag=Gamertag from UsersChars where CharID=@in_CharID
	if(@ClanID = 0) begin
		select 6 as ResultCode, 'no clan' as ResultMsg
		return
	end

	-- add members value is in permanent GD price
	declare @GPriceP int = 0
	select @GPriceP=GPriceP from Items_Generic where ItemID=@in_ItemID
	if(@GPriceP = 0) begin
		select 6 as ResultCode, 'no price1' as ResultMsg
		return
	end
	
	-- update clan
	update ClanData set MaxClanMembers=(MaxClanMembers+@GPriceP) where ClanID=@ClanID
	
	-- generate clan event
	insert into ClanEvents (
		ClanID,
		EventDate,
		EventType,
		EventRank,
		Var1,
		Var2,
		Text1
	) values (
		@ClanID,
		GETDATE(),
		13, -- ClanEvent_AddMaxMembers
		99, -- Visible to all
		@in_CharID,
		@GPriceP,
		@Gamertag
	)
	
	-- success
	select 0 as ResultCode

END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanApplyAnswer]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanApplyAnswer]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanApplyAnswer]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanApplyAnswer]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ClanApplyAnswer]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ClanApplyAnswer]
	@in_CharID int,
	@in_ClanApplicationID int,
	@in_Answer int
AS
BEGIN
	SET NOCOUNT ON;

-- sanity checks

	-- clan id valudation of caller
	declare @ClanID int = 0
	declare @ClanRank int
	declare @Gamertag nvarchar(64)
	select @ClanID=ClanID, @ClanRank=ClanRank, @Gamertag=Gamertag from UsersChars where CharID=@in_CharID
	if(@ClanID = 0) begin
		select 6 as ResultCode, 'no clan' as ResultMsg
		return
	end
	
	-- only leader and officers can answer application
	if(@ClanRank > 1) begin
		select 23 as ResultCode, 'no permission' as ResultMsg
		return
	end

	-- check if we have enough slots in clan
	declare @MaxClanMembers int
	declare @NumClanMembers int
	select @MaxClanMembers=MaxClanMembers, @NumClanMembers=NumClanMembers from ClanData where ClanID=@ClanID
	if(@NumClanMembers >= @MaxClanMembers) begin
		select 20 as 'ResultCode', 'not enough slots' as ResultMsg
		return
	end

-- check application

	declare @AppClanID int = 0
	declare @AppCharID int
	select @AppClanID=ClanID, @AppCharID=CharID	from ClanApplications where ClanApplicationID=@in_ClanApplicationID
	if(@AppClanID <> @ClanID) begin
		select 6 as ResultCode, 'bad application id' as ResultMsg
		return
	end
	
	-- mark that application as processed
	update ClanApplications set IsProcessed=1 where ClanApplicationID=@in_ClanApplicationID
	
	-- make sure that this guy isn't joined other clan somehow (race condition)
	declare @AppGamertag nvarchar(64)
	select @AppClanID=ClanID, @AppGamertag=Gamertag from UsersChars where CharID=@AppCharID
	if(@AppClanID <> 0) begin
		select 21 as ResultCode, 'applicant already joined clan' as ResultMsg
		return
	end

	if(@in_Answer = 0)
	begin
		-- declined clan joining
		-- TODO: send message to player about denial

		select 0 as ResultCode
		return
	end
	
	-- accept application, join player to clan
	update ClanData set NumClanMembers=(NumClanMembers + 1) where ClanID=@ClanID
	update UsersChars set ClanID=@ClanID, ClanRank=99 where CharID=@AppCharID
	
	-- clear all other applications
	delete from ClanApplications where CharID=@AppCharID

-- generate clan event
	insert into ClanEvents (
		ClanID,
		EventDate,
		EventType,
		EventRank,
		Var1,
		Text1
	) values (
		@ClanID,
		GETDATE(),
		4, -- CLANEvent_Join
		99, -- Visible to all
		@AppCharID,
		@AppGamertag
	)
	
	select 0 as ResultCode
	return

END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanApplyGetList]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanApplyGetList]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanApplyGetList]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanApplyGetList]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ClanApplyGetList]
-- ----------------------------


CREATE PROCEDURE [dbo].[WZ_ClanApplyGetList]
	@in_CharID int
AS
BEGIN
	SET NOCOUNT ON;

-- sanity checks

	-- clan id valudation of caller
	declare @ClanID int = 0
	declare @ClanRank int
	declare @Gamertag nvarchar(64)
	select @ClanID=ClanID, @ClanRank=ClanRank, @Gamertag=Gamertag from UsersChars where CharID=@in_CharID
	if(@ClanID = 0) begin
		select 6 as ResultCode, 'no clan' as ResultMsg
		return
	end
	
-- give list of applyers

	-- only leader and officers can view application list
	if(@ClanRank > 1) begin
		select 6 as ResultCode, 'no permission' as ResultMsg
		return
	end

	-- success
	select 0 as ResultCode
	
	select 
		a.ClanApplicationID,
		a.ApplicationText,
		DATEDIFF(mi, GETDATE(), a.ExpireTime) as MinutesLeft,
		c.*
	from ClanApplications a
	join UsersChars c on (c.CharID=a.CharID)
	where a.ClanID=@ClanID and GETDATE()<ExpireTime and IsProcessed=0
	
	return
	
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanApplyToJoin]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanApplyToJoin]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanApplyToJoin]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanApplyToJoin]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ClanApplyToJoin]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ClanApplyToJoin]
	@in_CharID int,
	@in_ClanID int,
	@in_ApplicationText nvarchar(500)
AS
BEGIN
	SET NOCOUNT ON;

	declare @APPLY_EXPIRE_TIME_HOURS int = 72
	declare @MAX_PENDING_APPS int = 5	-- can be maximum 5 pending invitations
	
-- sanity checks

	-- player must be without clan
	declare @PlayerClanID int = 0
	select @PlayerClanID=ClanID from UsersChars where CharID=@in_CharID
	if(@PlayerClanID > 0) begin
		select 6 as ResultCode, 'already in clan' as ResultMsg
		return
	end

	-- make sure clan exists
	if not exists (select ClanID from ClanData where ClanID=@in_ClanID) begin
		select 6 as ResultCode, 'no clanid' as ResultMsg
		return
	end
	
	-- see if we already have pending invidation
	declare @AppExpireTime datetime
	select @AppExpireTime=ExpireTime from ClanApplications where ClanID=@in_ClanID and CharID=@in_CharID and GETDATE()<ExpireTime
	if(@@ROWCOUNT > 0) begin
		select 24 as ResultCode, 'pending application' as ResultMsg
		return
	end
	
	-- see if we already have too much applications
	declare @AppTotalCounts int = 0
	select @AppTotalCounts=COUNT(*) from ClanApplications where CharID=@in_CharID and GETDATE()<ExpireTime
	if(@AppTotalCounts >= @MAX_PENDING_APPS) begin
		select 25 as ResultCode, 'too many applications' as ResultMsg
		return 
	end
	
-- send application

	insert into ClanApplications (
		ClanID,
		CharID,
		ExpireTime,
		ApplicationText,
		IsProcessed
	) values (
		@in_ClanID,
		@in_CharID,
		DATEADD(hour, @APPLY_EXPIRE_TIME_HOURS, GETDATE()),
		@in_ApplicationText,
		0
	)

	-- success
	select 0 as ResultCode
	return
	
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanCreate]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanCreate]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanCreate]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanCreate]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ClanCreate]
-- ----------------------------


CREATE PROCEDURE [dbo].[WZ_ClanCreate]
	@in_CustomerID int,
	@in_CharID int,
	@in_ClanName nvarchar(64),
	@in_ClanNameColor int,
	@in_ClanTag nvarchar(4),
	@in_ClanTagColor int,
	@in_ClanEmblemID int,
	@in_ClanEmblemColor int
AS
BEGIN
	SET NOCOUNT ON;
	
	declare @DEFAULT_CLAN_SIZE int = 15

	-- sanity check
	declare @ClanID int = 0
	declare @Gamertag nvarchar(64)
	select @ClanID=ClanID, @Gamertag=Gamertag from UsersChars where CharID=@in_CharID
	if(@ClanID > 0) begin
		select 6 as 'ResultCode', 'already have clan' as 'ResultMsg'
		return
	end
	
	-- check for 20 hour play time
	declare @TimePlayedHours int = 0
	select @TimePlayedHours=(TimePlayed/3600) from UsersData where CustomerID=@in_CustomerID

	-- create clan!
	insert into ClanData (
		ClanName, ClanNameColor, 
		ClanTag, ClanTagColor,
		ClanEmblemID, ClanEmblemColor,
		ClanXP,	ClanLevel, ClanGP,
		OwnerCustomerID, OwnerCharID,
		MaxClanMembers, NumClanMembers,
		ClanCreateDate
	) values (
		@in_ClanName, @in_ClanNameColor,
		@in_ClanTag, @in_ClanTagColor,
		@in_ClanEmblemID, @in_ClanEmblemColor,
		0,	0,	0,
		@in_CustomerID, @in_CharID,
		@DEFAULT_CLAN_SIZE,	1,
		GETDATE()
	)

	-- get new clanID
	select @ClanID=ClanID from ClanData where OwnerCharID=@in_CharID
	if(@@ROWCOUNT = 0) begin
		select 6 as 'ResultCode', 'clan creation failed!' as 'ResultMsg'
		return
	end
	
	-- update owner clan data
	update UsersChars set ClanID=@ClanID, ClanRank=0 where CharID=@in_CharID
	
	-- generate clan event
	insert into ClanEvents (
		ClanID,
		EventDate,
		EventType,
		EventRank,
		Var1,
		Text1
	) values (
		@ClanID,
		GETDATE(),
		1, -- CLANEVENT_Created
		99, -- Visible to all
		@in_CharID,
		@Gamertag
	)
	
	-- success
	select 0 as ResultCode
	
	select @ClanID as 'ClanID'
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanCreateCheckMoney]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanCreateCheckMoney]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanCreateCheckMoney]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanCreateCheckMoney]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ClanCreateCheckMoney]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ClanCreateCheckMoney]
	@in_CustomerID int
AS
BEGIN
	SET NOCOUNT ON;

	-- this call is always valid
	select 0 as ResultCode

	-- doesn't need money yet	
	select 0 as NeedMoney

	return
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanCreateCheckParams]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanCreateCheckParams]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanCreateCheckParams]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanCreateCheckParams]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ClanCreateCheckParams]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ClanCreateCheckParams]
	@in_CharID int,
	@in_ClanName nvarchar(64),
	@in_ClanTag nvarchar(4)
AS
BEGIN
	SET NOCOUNT ON;

	-- user can't create more that one clan
	declare @ClanID int = 0
	select @ClanID=ClanID from UsersChars where CharID=@in_CharID
	if(@ClanID > 0) begin
		select 6 as ResultCode, 'already have clan' as ResultMsg
		return
	end
	
	-- check that name/tag is unique
	if(exists(select * from ClanData where ClanName=@in_ClanName)) begin
		select 27 as ResultCode, 'clan name' as ResultMsg
		return
	end
	if(exists(select * from ClanData where ClanTag=@in_ClanTag)) begin
		select 28 as ResultCode, 'clan tag' as ResultMsg
		return
	end

	select 0 as ResultCode
	return
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanDonateToClanGP]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanDonateToClanGP]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanDonateToClanGP]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanDonateToClanGP]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ClanDonateToClanGP]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ClanDonateToClanGP]
	@in_CustomerID int,
	@in_CharID int,
	@in_GP int
AS
BEGIN
	SET NOCOUNT ON;
	
-- sanity checks
	declare @ClanID int
	declare @ClanRank int
	declare @Gamertag nvarchar(64)
	select @ClanID=ClanID, @ClanRank=ClanRank, @Gamertag=Gamertag from UsersChars where CharID=@in_CharID
	if(@ClanID = 0) begin
		select 6 as ResultCode, 'not in clan' as ResultMsg
		return
	end

	declare @GamePoints int = 0
	select @GamePoints=GamePoints from UsersData where CustomerID=@in_CustomerID
	
	if(@in_GP < 0) begin
		select 6 as ResultCode, 'sneaky bastard...' as ResultMsg
		return
	end
	if(@in_GP > @GamePoints) begin
		select 6 as ResultCode, 'not enough GP' as ResultMsg
		return
	end

-- donating

	-- substract GP
	declare @AlterGP int = -@in_GP
	exec FN_AlterUserGP @in_CustomerID, @AlterGP, 'toclan'
	update UsersChars set ClanContributedGP=(ClanContributedGP+@in_GP) where CharID=@in_CharID
	-- and record that
	INSERT INTO FinancialTransactions
		VALUES (@in_CustomerID, 'CLAN_GPToClan', 4000, GETDATE(), 
				@in_GP, '1', 'APPROVED', @ClanID)
	
	-- add clan gp
	update ClanData set ClanGP=(ClanGP+@in_GP) where ClanID=@ClanID

-- generate clan event
	insert into ClanEvents (
		ClanID,
		EventDate,
		EventType,
		EventRank,
		Var1,
		Var3,
		Text1
	) values (
		@ClanID,
		GETDATE(),
		10, -- CLANEvent_DonateToClanGP
		99, -- Visible to all
		@in_CharID,
		@in_GP,
		@Gamertag
	)

	-- success
	select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanDonateToMemberGP]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanDonateToMemberGP]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanDonateToMemberGP]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanDonateToMemberGP]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ClanDonateToMemberGP]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ClanDonateToMemberGP]
	@in_CharID int,
	@in_GP int,
	@in_MemberID int
AS
BEGIN
	SET NOCOUNT ON;
	
-- sanity checks

	-- clan id valudation of caller
	declare @ClanID int
	declare @ClanRank int
	declare @Gamertag nvarchar(64)
	select @ClanID=ClanID, @ClanRank=ClanRank, @Gamertag=Gamertag from UsersChars where CharID=@in_CharID
	if(@ClanID = 0) begin
		select 6 as ResultCode, 'not in clan' as ResultMsg
		return
	end

	-- clan id validation of member
	declare @MemberClanID int = 0
	declare @MemberGamerTag nvarchar(64)
	select @MemberClanID=ClanID, @MemberGamerTag=GamerTag from UsersChars where CharID=@in_MemberID
	if(@MemberClanID <> @ClanID) begin
		select 6 as ResultCode, 'member in wrong clan' as ResultMsg
		return
	end
	
-- donating
	if(@ClanRank > 0) begin
		select 23 as ResultCode, 'no permission' as ResultMsg
		return
	end
	
	declare @ClanGP int = 0
	select @ClanGP=ClanGP from ClanData where ClanID=@ClanID
	if(@in_GP < 0) begin
		select 6 as ResultCode, 'sneaky bastard...' as ResultMsg
		return
	end
	if(@in_GP > @ClanGP) begin
		select 6 as ResultCode, 'not enough GP in clan' as ResultMsg
		return
	end

	-- substract GP from clan
	update ClanData set ClanGP=(ClanGP-@in_GP) where ClanID=@ClanID

	-- add member gp
	exec FN_AlterUserGP @in_MemberID, @in_GP, 'fromclan'
	-- and record that
	INSERT INTO FinancialTransactions
		VALUES (@in_MemberID, 'CLAN_GPToMember', 4001, GETDATE(), 
				@in_GP, '1', 'APPROVED', @ClanID)
	
-- generate clan event
	insert into ClanEvents (
		ClanID,
		EventDate,
		EventType,
		EventRank,
		Var1,
		Var2,
		Var3,
		Text1,
		Text2
	) values (
		@ClanID,
		GETDATE(),
		11, -- CLANEvent_DonateToMemberGP
		1, -- Visible to officers
		@in_CharID,
		@in_MemberID,
		@in_GP,
		@Gamertag,
		@MemberGamertag
	)
	
-- TODO: send message to player about donate

	-- success
	select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanFN_DeleteClan]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanFN_DeleteClan]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanFN_DeleteClan]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanFN_DeleteClan]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ClanFN_DeleteClan]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ClanFN_DeleteClan]
	@in_ClanID int
AS
BEGIN
	SET NOCOUNT ON;
	
	delete from ClanData where ClanID=@in_ClanID
	delete from ClanApplications where ClanID=@in_ClanID
	delete from ClanInvites where ClanID=@in_ClanID

	update UsersChars set ClanID=0 where ClanID=@in_ClanID
	
	return
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanGetEvents]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanGetEvents]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanGetEvents]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanGetEvents]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ClanGetEvents]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ClanGetEvents]
	@in_CharID int,
	@in_Days int
AS
BEGIN
	SET NOCOUNT ON;

-- sanity checks

	-- clan id valudation of caller
	declare @ClanID int = 0
	declare @ClanRank int
	declare @Gamertag nvarchar(64)
	select @ClanID=ClanID, @ClanRank=ClanRank, @Gamertag=Gamertag from UsersChars where CharID=@in_CharID
	if(@ClanID = 0) begin
		select 6 as ResultCode, 'no clan' as ResultMsg
		return
	end
	
-- report clan log
	select 0 as ResultCode
	
	declare @MinDate datetime = DATEADD(day, -@in_Days, GETDATE())
	select * from ClanEvents where ClanID=@ClanID and EventDate>=@MinDate and @ClanRank <= EventRank order by EventDate asc
	
	return
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanGetInfo]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanGetInfo]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanGetInfo]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanGetInfo]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ClanGetInfo]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ClanGetInfo]
	@in_ClanID int,
	@in_GetMembers int
AS
BEGIN
	SET NOCOUNT ON;

	-- success
	select 0 as ResultCode
	
	-- and report clan data
	if(@in_ClanID > 0) 
	begin
		-- specific clan
		select *, c.gamertag from ClanData 
			left join UsersChars c on c.CharID=ClanData.OwnerCharID
			where ClanData.ClanID=@in_ClanID
	end
	else 
	begin
		-- all clans
		select *, c.gamertag from ClanData
			left join UsersChars c on c.CharID=ClanData.OwnerCharID
	end
		
	-- if need to report members
	if(@in_ClanID > 0 and @in_GetMembers > 0) begin
		select UsersChars.* from UsersChars	where ClanID=@in_ClanID
	end

	return
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanGetPlayerData]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanGetPlayerData]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanGetPlayerData]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanGetPlayerData]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ClanGetPlayerData]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ClanGetPlayerData]
	@in_CharID int
AS
BEGIN
	SET NOCOUNT ON;

	-- success
	select 0 as ResultCode
	
	-- report player clan id and current clan info
	select c.ClanID, c.ClanRank, d.*
		from UsersChars c
		left join ClanData d on d.ClanID=c.ClanID
		where CharID=@in_CharID

	return
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanInviteAnswer]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanInviteAnswer]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanInviteAnswer]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanInviteAnswer]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ClanInviteAnswer]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ClanInviteAnswer]
	@in_CharID int,
	@in_ClanInviteID int,
	@in_Answer int
AS
BEGIN
	SET NOCOUNT ON;

-- sanity checks

	-- must be free to join clan
	declare @ClanID int = 0
	declare @Gamertag nvarchar(64)
	select @ClanID=ClanID, @Gamertag=Gamertag from UsersChars where CharID=@in_CharID
	if(@ClanID <> 0) begin
		select 6 as ResultCode, 'already in clan' as ResultMsg
		return
	end
	
	-- have valid invitation id (get actual ClanID here)
	declare @InvCharID int
	select @ClanID=ClanID, @InvCharID=CharID from ClanInvites where ClanInviteID=@in_ClanInviteID
	if(@@ROWCOUNT = 0) begin
		select 6 as ResultCode, 'bad inviteid #1' as ResultMsg
		return
	end
	if(@InvCharID <> @in_CharID) begin
		select 6 as ResultCode, 'bad inviteid #2' as ResultMsg
		return
	end

-- invite

	-- delete invite anyway
	delete from ClanInvites where ClanInviteID=@in_ClanInviteID
	
	-- check if invite is denied
	if(@in_Answer = 0) begin
		select 0 as ResultCode
		select @ClanID as ClanID
		return
	end

	-- check if we have enough slots in clan
	declare @MaxClanMembers int
	declare @NumClanMembers int
	select @MaxClanMembers=MaxClanMembers, @NumClanMembers=NumClanMembers from ClanData where ClanID=@ClanID
	if(@NumClanMembers >= @MaxClanMembers) begin
		select 20 as 'ResultCode', 'not enough slots' as ResultMsg
		return
	end
	
	-- join the clan!
	update ClanData set NumClanMembers=(NumClanMembers + 1) where ClanID=@ClanID
	update UsersChars set ClanID=@ClanID, ClanRank=99 where CharID=@in_CharID

-- generate clan event
	insert into ClanEvents (
		ClanID,
		EventDate,
		EventType,
		EventRank,
		Var1,
		Text1
	) values (
		@ClanID,
		GETDATE(),
		4, -- CLANEvent_Join
		99, -- Visible to officers
		@in_CharID,
		@Gamertag
	)
	
	-- success
	select 0 as ResultCode
	select @ClanID as ClanID
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanInviteGetInvitesForPlayer]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanInviteGetInvitesForPlayer]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanInviteGetInvitesForPlayer]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanInviteGetInvitesForPlayer]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ClanInviteGetInvitesForPlayer]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ClanInviteGetInvitesForPlayer]
	@in_CharID int
AS
BEGIN
	SET NOCOUNT ON;

-- report all pending invites

	select 0 as ResultCode
	
	select 
		i.ClanInviteID,
		c.Gamertag, 
		d.*
	from ClanInvites i
	left join UsersChars c on (c.CharID=i.InviterCharID)
	join ClanData d on (d.ClanID=i.ClanID)
	where i.CharID=@in_CharID and GETDATE()<ExpireTime
	
	return
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanInviteReportAll]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanInviteReportAll]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanInviteReportAll]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanInviteReportAll]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ClanInviteReportAll]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ClanInviteReportAll]
	@in_CharID int
AS
BEGIN
	SET NOCOUNT ON;

-- sanity checks

	-- clan id valudation of caller
	declare @ClanID int = 0
	declare @ClanRank int
	declare @Gamertag nvarchar(64)
	select @ClanID=ClanID, @ClanRank=ClanRank, @Gamertag=Gamertag from UsersChars where CharID=@in_CharID
	if(@ClanID = 0) begin
		select 6 as ResultCode, 'no clan' as ResultMsg
		return
	end
	
-- validate that we can invite

	-- only leader and officers can invite
	if(@ClanRank > 1) begin
		select 6 as ResultCode, 'no permission' as ResultMsg
		return
	end

-- report all pending invites

	-- success
	select 0 as ResultCode
	
	select 
		i.ClanInviteID, 
		c.Gamertag,
		DATEDIFF(mi, GETDATE(), i.ExpireTime) as MinutesLeft
	from ClanInvites i
	join UsersChars c on (c.CharID=i.CharID)
	where i.ClanID=@ClanID and GETDATE()<ExpireTime
	
	return
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanInviteSendToPlayer]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanInviteSendToPlayer]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanInviteSendToPlayer]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanInviteSendToPlayer]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ClanInviteSendToPlayer]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ClanInviteSendToPlayer]
	@in_CharID int,
	@in_InvGamertag nvarchar(64)
AS
BEGIN
	SET NOCOUNT ON;

	declare @INVITE_EXPIRE_TIME_HOURS int = 72
	
-- sanity checks

	-- clan id valudation of caller
	declare @ClanID int = 0
	declare @ClanRank int
	declare @Gamertag nvarchar(64)
	select @ClanID=ClanID, @ClanRank=ClanRank, @Gamertag=Gamertag from UsersChars where CharID=@in_CharID
	if(@ClanID = 0) begin
		select 6 as ResultCode, 'not in clan' as ResultMsg
		return
	end
	
-- validate that we can invite

	-- only leader and officers can invite
	if(@ClanRank > 1) begin
		select 23 as ResultCode, 'no permission' as ResultMsg
		return
	end
	
	-- check if we have enough slots in clan
	declare @MaxClanMembers int
	declare @NumClanMembers int
	select @MaxClanMembers=MaxClanMembers, @NumClanMembers=NumClanMembers from ClanData where ClanID=@ClanID
	
	declare @PendingInvites int = 0
	--DISABLED FOR NOW: select @PendingInvites=COUNT(*) from ClanInvites where ClanID=@ClanID and GETDATE()<ExpireTime
	if((@NumClanMembers + @PendingInvites) >= @MaxClanMembers) begin
		select 20 as 'ResultCode', 'not enough slots' as ResultMsg
		return
	end

	-- check if user exists	
	declare @InvCharID int
	declare @InvClanID int
	select @InvCharID=CharID, @InvClanID=ClanID from UsersChars where Gamertag=@in_InvGamertag
	if(@@ROWCOUNT = 0) begin
		select 22 as ResultCode, 'no such gamertag' as ResultMsg
		return
	end
	-- and have no clan
	if(@InvClanID <> 0) begin
		select 21 as ResultCode, 'already in clan' as ResultMsg
		return
	end
	
	-- check if we have pending invite
	if(exists(select * from ClanInvites where ClanID=@ClanID and CharID=@InvCharID and GETDATE()<ExpireTime)) begin
		select 24 as ResultCode, 'already invited' as ResultMsg
		return
	end
	
-- invite
	insert into ClanInvites (
		ClanID,
		InviterCharID,
		CharID,
		ExpireTime
	) values (
		@ClanID,
		@in_CharID,
		@InvCharID,
		DATEADD(hour, @INVITE_EXPIRE_TIME_HOURS, GETDATE())
	)

	-- success
	select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanKickMember]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanKickMember]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanKickMember]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanKickMember]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ClanKickMember]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ClanKickMember]
	@in_CharID int,
	@in_MemberID int
AS
BEGIN
	SET NOCOUNT ON;
	
-- sanity checks
	if(@in_CharID = @in_MemberID) begin
		select 6 as 'ResultCode', 'cant kick himselft' as 'ResultMsg'
		return
	end

	-- clan id valudation of caller
	declare @ClanID int = 0
	declare @ClanRank int
	declare @Gamertag nvarchar(64)
	select @ClanID=ClanID, @ClanRank=ClanRank, @Gamertag=Gamertag from UsersChars where CharID=@in_CharID
	if(@ClanID = 0) begin
		select 6 as ResultCode, 'no clan' as ResultMsg
		return
	end
	
	-- clan id validation of member
	declare @MemberClanID int = 0
	declare @MemberGamerTag nvarchar(64)
	declare @MemberClanRank int
	select @MemberClanID=ClanID, @MemberClanRank=ClanRank, @MemberGamerTag=GamerTag from UsersChars where CharID=@in_MemberID
	if(@MemberClanID <> @ClanID) begin
		select 6 as ResultCode, 'member in wrong clan' as ResultMsg
		return
	end
	
-- validate that we can kick

	-- only leader and officers can kick
	if(@ClanRank > 1) begin
		select 23 as ResultCode, 'no permission' as ResultMsg
		return
	end

	-- cant kick higher rank
	if(@ClanRank > 0 and @ClanRank >= @MemberClanRank) begin
		select 6 as ResultCode, 'cant kick highter rank' as ResultMsg
		return
	end
	
-- update clan info and kick player
	update ClanData set NumClanMembers=(NumClanMembers-1) where ClanID=@ClanID
	update UsersChars set ClanID=0, ClanContributedGP=0, ClanContributedXP=0 where CharID=@in_MemberID
	
-- generate clan event
	insert into ClanEvents (
		ClanID,
		EventDate,
		EventType,
		EventRank,
		Var1,
		Var2,
		Text1,
		Text2
	) values (
		@ClanID,
		GETDATE(),
		6, -- CLANEvent_Kick
		99, -- Visible to all
		@in_CharID,
		@in_MemberID,
		@Gamertag,
		@MemberGamertag
	)
	
	-- TODO: send message to player about kick
	
	-- success
	select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanLeave]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanLeave]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanLeave]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanLeave]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ClanLeave]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ClanLeave]
	@in_CharID int
AS
BEGIN
	SET NOCOUNT ON;
	
-- sanity checks
	declare @ClanID int
	declare @ClanRank int
	declare @Gamertag nvarchar(64)
	select @ClanID=ClanID, @ClanRank=ClanRank, @Gamertag=Gamertag from UsersChars where CharID=@in_CharID
	if(@ClanID = 0) begin
		select 6 as ResultCode, 'no clan' as ResultMsg
		return
	end
	
-- leader is leaving clan	
	if(@ClanRank = 0) 
	begin
		declare @NumClanMembers int
		select @NumClanMembers=COUNT(*) from UsersChars where ClanID=@ClanID
		if(@NumClanMembers > 1) begin
			select 6 as ResultCode, 'owner cant leave - there is people left in clan' as ResultMsg
			return
		end

		-- generate clan event
		insert into ClanEvents (
			ClanID,
			EventDate,
			EventType,
			EventRank,
			Var1,
			Text1
		) values (
			@ClanID,
			GETDATE(),
			99, -- CLANEvent_Disband
			99, -- Visible to all
			@in_CharID,
			@Gamertag
		)
		
		-- and delete clan
		exec WZ_ClanFN_DeleteClan @ClanID
		
		select 0 as ResultCode
		return
	end

	-- actual leave
	update UsersChars set ClanID=0, ClanContributedGP=0, ClanContributedXP=0 where CharID=@in_CharID
	update ClanData set NumClanMembers=(NumClanMembers - 1) where ClanID=@ClanID

-- generate clan event
	insert into ClanEvents (
		ClanID,
		EventDate,
		EventType,
		EventRank,
		Var1,
		Text1
	) values (
		@ClanID,
		GETDATE(),
		5, -- CLANEvent_Left
		99, -- Visible to all
		@in_CharID,
		@Gamertag
	)
	
	-- success
	select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanSetLore]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanSetLore]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanSetLore]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanSetLore]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ClanSetLore]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ClanSetLore]
	@in_CharID int,
	@in_Lore nvarchar(512)
AS
BEGIN
	SET NOCOUNT ON;
	
	-- clan id valudation of caller
	declare @ClanID int = 0
	declare @ClanRank int = 99
	declare @Gamertag nvarchar(64)
	select @ClanID=ClanID, @ClanRank=ClanRank, @Gamertag=Gamertag from UsersChars where CharID=@in_CharID
	if(@ClanID = 0) begin
		select 6 as ResultCode, 'no clan' as ResultMsg
		return
	end
	
	-- only leader and officers can change lore
	if(@ClanRank > 1) begin
		select 23 as ResultCode, 'no permission' as ResultMsg
		return
	end
	
	update ClanData set ClanLore=@in_Lore where ClanID=@ClanID

	-- success
	select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanSetMemberRank]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanSetMemberRank]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanSetMemberRank]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ClanSetMemberRank]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_ClanSetMemberRank]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_ClanSetMemberRank]
	@in_CharID int,
	@in_MemberID int,
	@in_Rank int
AS
BEGIN
	SET NOCOUNT ON;
	
-- sanity checks
	if(@in_CharID = @in_MemberID) begin
		select 6 as 'ResultCode', 'can not set own rank' as 'ResultMsg'
		return
	end

	-- clan id valudation of caller
	declare @ClanID int = 0
	declare @ClanRank int
	declare @Gamertag nvarchar(64)
	select @ClanID=ClanID, @ClanRank=ClanRank, @Gamertag=Gamertag from UsersChars where CharID=@in_CharID
	if(@ClanID = 0) begin
		select 6 as ResultCode, 'no clan' as ResultMsg
		return
	end
	
	-- clan id validation of member
	declare @MemberClanID int = 0
	declare @MemberGamerTag nvarchar(64)
	declare @MemberClanRank int
	declare @MemberCustomerID int
	select @MemberClanID=ClanID, @MemberClanRank=ClanRank, @MemberGamerTag=GamerTag, @MemberCustomerID=CustomerID from UsersChars where CharID=@in_MemberID
	if(@MemberClanID <> @ClanID) begin
		select 6 as ResultCode, 'member in wrong clan' as ResultMsg
		return
	end
	
-- validate that we can change rank

	-- only leader and officers can change ranks
	if(@ClanRank > 1) begin
		select 23 as ResultCode, 'no permission' as ResultMsg
		return
	end

	-- cant change higher rank
	if(@ClanRank > 0 and @ClanRank >= @MemberClanRank) begin
		select 6 as ResultCode, 'cant change highter rank' as ResultMsg
		return
	end
	
	if(@ClanRank > 0 and @ClanRank >= @in_Rank) begin
		select 6 as ResultCode, 'cant set same rank' as ResultMsg
		return
	end
	
-- code for changing clan ownership, owner becomes officer
	if(@ClanRank = 0 and @in_Rank = 0) begin
		update UsersChars set ClanRank=1 where CharID=@in_CharID
		update ClanData set OwnerCharID=@in_MemberID, OwnerCustomerID=@MemberCustomerID where ClanID=@ClanID
	end
	
-- update target member
	update UsersChars set ClanRank=@in_Rank where CharID=@in_MemberID
	
-- generate clan set rank event
	insert into ClanEvents (
		ClanID,
		EventDate,
		EventType,
		EventRank,
		Var1,
		Var2,
		Var3,
		Text1,
		Text2
	) values (
		@ClanID,
		GETDATE(),
		3, -- CLANEVENT_SetRank
		99, -- Visible to all
		@in_CharID,
		@in_MemberID,
		@in_Rank,
		@Gamertag,
		@MemberGamertag
	)
	
	-- TODO: send message to player about rank change
	
	-- success
	select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ConvertGD]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ConvertGD]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_ConvertGD]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_ConvertGD]
-- ----------------------------
CREATE PROCEDURE [dbo].[WZ_ConvertGD]
@in_CustomerID int,
	@in_Var1 int,
	@in_Var2 int
AS
BEGIN
	declare @GamePoints int
	SELECT @GamePoints=GamePoints FROM UsersData WHERE CustomerID=@in_CustomerID

	if(@GamePoints < @in_Var1) begin
		select 7 as ResultCode, 'Not Enough GP' as ResultMsg
		return
	end
update UsersData set GamePoints=(GamePoints-@in_Var1) WHERE CustomerID=@in_CustomerID
update UsersData set GameDollars=(GameDollars+@in_Var2) WHERE CustomerID=@in_CustomerID
INSERT INTO FinancialTransactions
		VALUES (@in_CustomerID, 'ConvertGCToGD', '3000', GETDATE(), 
				@in_var1, '1', 'APPROVED', @in_Var2)
select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_DB_GenerateDailyLeaderboard]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_DB_GenerateDailyLeaderboard]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_DB_GenerateDailyLeaderboard]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_DB_GenerateDailyLeaderboard]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_DB_GenerateDailyLeaderboard]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_DB_GenerateDailyLeaderboard]
AS  
BEGIN  
	SET NOCOUNT ON;  
	
	/*
	The Query Processor estimates that implementing the following index could improve the query cost by 13.035%.
	NOTE: using index IX_DBG_UserRoundResults_LBIdx1 on DBG_UserRoundResults
	*/

	declare @CurDay date = GETDATE() -- date is truncated to day only, no time

	-- reset table with zero seed
	delete from Leaderboard1
	DBCC CHECKIDENT (Leaderboard1, RESEED, 0)
	
	-- insert all ordered by honorpoints
	insert into Leaderboard1 (
			CustomerID, gamertag, HonorPoints,
			Kills, Deaths, Wins, Losses, ShotsFired, ShotsHit, 
			TimePlayed,
			Rank,
			HavePremium)
		select
			urr.CustomerID,
			l.Gamertag,
			sum(urr.HonorPoints),
			sum(Kills), sum(Deaths), sum(Wins), sum(Losses), sum(ShotsFired), sum(ShotsHits),
			sum(TimePlayed),
			-- not need rank now
			0, --(select top(1) rank from DataRankPoints where LoginID.HonorPoints<DataRankPoints.HonorPoints order by HonorPoints asc),
			-- check if have premium
			(case when exists (select * from Inventory where ItemID=301004 and Inventory.CustomerID=urr.CustomerID and LeasedUntil>GETDATE()) 
				then 1
				else 0
			end)
		from DBG_UserRoundResults urr
		join LoginID l on (l.CustomerID=urr.CustomerID)
		where GameReportTime>=@CurDay and l.AccountStatus=101
		group by urr.CustomerID, l.Gamertag
		order by sum(urr.HonorPoints) desc
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_DB_GenerateLeaderboard]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_DB_GenerateLeaderboard]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_DB_GenerateLeaderboard]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_DB_GenerateLeaderboard]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_DB_GenerateLeaderboard]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_DB_GenerateLeaderboard]
AS  
BEGIN  
	SET NOCOUNT ON;  
	
	-- reset table with zero seed
	delete from Leaderboard
	DBCC CHECKIDENT (Leaderboard, RESEED, 0)
	
	-- insert all ordered by honorpoints
	insert into Leaderboard (
			CustomerID, gamertag, HonorPoints,
			Kills, Deaths, Wins, Losses ,ShotsFired, ShotsHit, 
			TimePlayed,
			Rank,
			HavePremium)
		select
			LoginID.CustomerID, gamertag, HonorPoints,
			Stats.Kills, Stats.Deaths, Stats.Wins, Stats.Losses, Stats.ShotsFired, Stats.ShotsHits, 
			Stats.TimePlayed,
			-- get rank from table
			(select top(1) rank from DataRankPoints where LoginID.HonorPoints<DataRankPoints.HonorPoints order by HonorPoints asc),
			-- check if have premium
			(case when exists (select * from Inventory where ItemID=301004 and Inventory.CustomerID=LoginID.CustomerID and LeasedUntil>GETDATE()) 
				then 1
				else 0
			end)
		from LoginID
		join Stats on Stats.CustomerID=LoginID.CustomerID
		where AccountStatus=101
		order by LoginID.HonorPoints desc

END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_DB_GenerateLeaderboard30]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_DB_GenerateLeaderboard30]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_DB_GenerateLeaderboard30]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_DB_GenerateLeaderboard30]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_DB_GenerateLeaderboard30]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_DB_GenerateLeaderboard30]
AS  
BEGIN  
	SET NOCOUNT ON;  

	/*
	The Query Processor estimates that implementing the following index could improve the query cost by 13.035%.
	NOTE: using index IX_DBG_UserRoundResults_LBIdx1 on DBG_UserRoundResults
	*/
	
	-- assemble start date of this month
	declare @StartDay date = '2000-01-01'
	set @StartDay = DATEADD(year, YEAR(GETDATE()) - 2000, @StartDay)
	set @StartDay = DATEADD(month, MONTH(GETDATE()) - 1, @StartDay)

	-- reset table with zero seed
	delete from Leaderboard30
	DBCC CHECKIDENT (Leaderboard30, RESEED, 0)
	
	-- insert all ordered by honorpoints
	insert into Leaderboard30 (
			CustomerID, gamertag, HonorPoints,
			Kills, Deaths, Wins, Losses, ShotsFired, ShotsHit, 
			TimePlayed,
			Rank,
			HavePremium)
		select
			urr.CustomerID,
			l.Gamertag,
			sum(urr.HonorPoints),
			sum(Kills), sum(Deaths), sum(Wins), sum(Losses), sum(ShotsFired), sum(ShotsHits),
			sum(TimePlayed),
			-- not need rank now
			0, --(select top(1) rank from DataRankPoints where LoginID.HonorPoints<DataRankPoints.HonorPoints order by HonorPoints asc),
			-- check if have premium
			(case when exists (select * from Inventory where ItemID=301004 and Inventory.CustomerID=urr.CustomerID and LeasedUntil>GETDATE()) 
				then 1
				else 0
			end)
		from DBG_UserRoundResults urr
		join LoginID l on (l.CustomerID=urr.CustomerID)
		where GameReportTime>=@StartDay and l.AccountStatus=101
		group by urr.CustomerID, l.Gamertag
		order by sum(urr.HonorPoints) desc
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_DB_GenerateLeaderboard7]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_DB_GenerateLeaderboard7]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_DB_GenerateLeaderboard7]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_DB_GenerateLeaderboard7]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_DB_GenerateLeaderboard7]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_DB_GenerateLeaderboard7]
AS  
BEGIN  
	SET NOCOUNT ON;  

	/*
	The Query Processor estimates that implementing the following index could improve the query cost by 13.035%.
	NOTE: using index IX_DBG_UserRoundResults_LBIdx1 on DBG_UserRoundResults
	*/
	
	-- assemble start date of this week
	declare @StartDay date = GETDATE()
	set @StartDay = DATEADD(day, -(DATEPART(weekday, @StartDay) - 1), @StartDay)

	-- reset table with zero seed
	delete from Leaderboard7
	DBCC CHECKIDENT (Leaderboard7, RESEED, 0)
	
	-- insert all ordered by honorpoints
	insert into Leaderboard7 (
			CustomerID, gamertag, HonorPoints,
			Kills, Deaths, Wins, Losses, ShotsFired, ShotsHit, 
			TimePlayed,
			Rank,
			HavePremium)
		select
			urr.CustomerID, 
			l.Gamertag,
			sum(urr.HonorPoints),
			sum(Kills), sum(Deaths), sum(Wins), sum(Losses), sum(ShotsFired), sum(ShotsHits),
			sum(TimePlayed),
			-- not need rank now
			0, --(select top(1) rank from DataRankPoints where LoginID.HonorPoints<DataRankPoints.HonorPoints order by HonorPoints asc),
			-- check if have premium
			(case when exists (select * from Inventory where ItemID=301004 and Inventory.CustomerID=urr.CustomerID and LeasedUntil>GETDATE()) 
				then 1
				else 0
			end)
		from DBG_UserRoundResults urr
		join LoginID l on (l.CustomerID=urr.CustomerID)
		where GameReportTime>=@StartDay and l.AccountStatus=101
		group by urr.CustomerID, l.Gamertag
		order by sum(urr.HonorPoints) desc
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetAccountInfo1]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetAccountInfo1]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetAccountInfo1]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetAccountInfo1]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_GetAccountInfo1]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_GetAccountInfo1] 
	@in_CustomerID int,
	@in_CharID int
AS
BEGIN
	SET NOCOUNT ON;

	-- check if CustomerID is valid
	if not exists (SELECT CustomerID FROM UsersData with(nolock) WHERE CustomerID=@in_CustomerID)
	begin
		select 6 as ResultCode
		return;
	end
	
	if(@in_CharID > 0) begin
		update UsersData set lastjoineddate=GETDATE() where CustomerID=@in_CustomerID
	end

	select 0 as ResultCode

	SELECT 
		UsersData.*,
		DATEDIFF(ss, lastgamedate, GETDATE()) as 'SecFromLastGame',
		ClanData.ClanTag, ClanData.ClanTagColor
	FROM UsersData with(nolock)
	left JOIN ClanData on (UsersData.ClanID = ClanData.ClanID)
	where UsersData.CustomerID=@in_CustomerID
	
--
-- report chars
--
	if(@in_CharID > 0) 
	begin
		-- single character, version called from server
		select 
			0 as 'SecToRevive', 
			* 
		from UsersChars with(nolock) where CustomerID=@in_CustomerID and CharID=@in_CharID
	end
	else 
	begin
		-- note that revive timer is 1hrs, change in WZ_CharRevive as well
		select 
	DATEDIFF (second, GETUTCDATE (), DATEADD (second, 600, DeathUtcTime)) as 'SecToRevive',
			* 
		from UsersChars with(nolock) where CustomerID=@in_CustomerID order by CharID asc
	end

--
-- report inventory
--
	select *
	from UsersInventory with(nolock)
	where CustomerID=@in_CustomerID and CharID=0
	
--
-- report backpacks
--
	if(@in_CharID > 0) begin
		-- single character, called from server
		select * from UsersInventory with(nolock) where CharID=@in_CharID
	end
	else begin
		select * from UsersInventory with(nolock) where CustomerID=@in_CustomerID and CharID>0 order by CharID asc
	end

END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetAccountInfo2]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetAccountInfo2]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetAccountInfo2]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetAccountInfo2]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_GetAccountInfo2]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_GetAccountInfo2] 
	@in_CustomerID int,
	@in_CharID int
AS
BEGIN
	SET NOCOUNT ON;

	-- check if CustomerID is valid
	if not exists (SELECT CustomerID FROM UsersData with(nolock) WHERE CustomerID=@in_CustomerID)
	begin
		select 6 as ResultCode
		return;
	end
	
	if(@in_CharID > 0) begin
		update UsersData set lastjoineddate=GETDATE() where CustomerID=@in_CustomerID
	end

	select 0 as ResultCode

	SELECT 
		UsersData.*,
		DATEDIFF(ss, lastgamedate, GETDATE()) as 'SecFromLastGame'
	FROM UsersData with(nolock)
	where UsersData.CustomerID=@in_CustomerID
	
--
-- report chars
--
	if(@in_CharID > 0) 
	begin
		-- single character, version called from server
		select 
			0 as 'SecToRevive', 
			c.*,
			ClanData.ClanTag, ClanData.ClanTagColor
		from UsersChars c with(nolock)
		left JOIN ClanData on (c.ClanID = ClanData.ClanID)
		where CustomerID=@in_CustomerID and CharID=@in_CharID
	end
	else 
	begin
		-- note that revive timer is 1hrs, change in WZ_CharRevive as well
declare @isPremium int = 0
  SELECT @isPremium=isPremium FROM UsersData WHERE CustomerID=@in_CustomerID

if (@isPremium = 1)
begin

   	declare @Premium int
	select -- second, 1 day * 30
		@Premium=DATEDIFF (second, GETUTCDATE (), DATEADD (SECOND, 0, PremiumExpireDate))
	from UsersData where CustomerID=@in_CustomerID

if (@Premium < 20)
BEGIN
UPDATE UsersData SET isPremium=0 where CustomerID=@in_CustomerID
end


		select 
DATEDIFF (second, GETUTCDATE (), DATEADD (second, 960, DeathUtcTime)) as 'SecToRevive',
			c.*,
			ClanData.ClanTag, ClanData.ClanTagColor
		from UsersChars c with(nolock)
		left JOIN ClanData on (c.ClanID = ClanData.ClanID)
		where CustomerID=@in_CustomerID order by CharID asc
END
ELSE
BEGIN
		select 
DATEDIFF (second, GETUTCDATE (), DATEADD (second, 1860, DeathUtcTime)) as 'SecToRevive',
			c.*,
			ClanData.ClanTag, ClanData.ClanTagColor
		from UsersChars c with(nolock)
		left JOIN ClanData on (c.ClanID = ClanData.ClanID)
		where CustomerID=@in_CustomerID order by CharID asc
	end
end

--
-- report inventory
--
	select *
	from UsersInventory with(nolock)
	where CustomerID=@in_CustomerID and CharID=0

-- select * from DBG_GPTransactions Where CustomerID=@in_CustomerID
	
--
-- report backpacks
--
	if(@in_CharID > 0) begin
		-- single character, called from server
		select * from UsersInventory with(nolock) where CharID=@in_CharID
	end
	else begin
		select * from UsersInventory with(nolock) where CustomerID=@in_CustomerID and CharID>0 order by CharID asc
	end
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetAccountInfo2_New]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetAccountInfo2_New]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetAccountInfo2_New]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetAccountInfo2_New]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_GetAccountInfo2]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_GetAccountInfo2_New] 
	@in_CustomerID int,
	@in_CharID int
AS
BEGIN
	SET NOCOUNT ON;

	-- check if CustomerID is valid
	if not exists (SELECT CustomerID FROM UsersData WHERE CustomerID=@in_CustomerID)
	begin
		select 6 as ResultCode
		return;
	end
	SELECT IsJoinPrivate FROM UsersPrivate WHERE CustomerID=@in_CustomerID
	SELECT PrivateServerName FROM UsersPrivate WHERE CustomerID=@in_CustomerID
	if(@in_CharID > 0) begin
		update UsersData set lastjoineddate=GETDATE() where CustomerID=@in_CustomerID
	end

	select 0 as ResultCode

	SELECT 
		UsersData.*,
		DATEDIFF(ss, lastgamedate, GETDATE()) as 'SecFromLastGame'
	FROM UsersData 
	where UsersData.CustomerID=@in_CustomerID
	
--
-- report chars
--
	if(@in_CharID > 0) 
	begin
		-- single character, version called from server
		select 
			0 as 'SecToRevive', 
			c.*,
			ClanData.ClanTag, ClanData.ClanTagColor
		from UsersChars c
		left JOIN ClanData on (c.ClanID = ClanData.ClanID)
		where CustomerID=@in_CustomerID and CharID=@in_CharID
	end
	else 
	begin
		-- note that revive timer is 1hrs, change in WZ_CharRevive as well
		select 
DATEDIFF (second, GETUTCDATE (), DATEADD (second, 960, DeathUtcTime)) as 'SecToRevive',
			c.*,
			ClanData.ClanTag, ClanData.ClanTagColor
		from UsersChars c
		left JOIN ClanData on (c.ClanID = ClanData.ClanID)
		where CustomerID=@in_CustomerID order by CharID asc
	end

--
-- report inventory
--
	select *
	from UsersInventory
	where CustomerID=@in_CustomerID and CharID=0
	
--
-- report backpacks
--
	if(@in_CharID > 0) begin
		-- single character, called from server
		select * from UsersInventory where CharID=@in_CharID
	end
	else begin
		select * from UsersInventory where CustomerID=@in_CustomerID and CharID>0 order by CharID asc
	end

END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetDataGameRewards]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetDataGameRewards]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetDataGameRewards]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetDataGameRewards]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_GetDataGameRewards]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_GetDataGameRewards]
AS
BEGIN  
	SET NOCOUNT ON;  

	select 0 as ResultCode
	select * from DataGameRewards
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetItemsData]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetItemsData]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetItemsData]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetItemsData]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_GetItemsData]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_GetItemsData] 
AS
BEGIN
	SET NOCOUNT ON;

	select 0 as ResultCode
	
	select * from Items_Gear
	select * from Items_Weapons;
	select * from Items_Generic
	
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetShopInfo1]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetShopInfo1]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetShopInfo1]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_GetShopInfo1]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_GetShopInfo1]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_GetShopInfo1] 
AS
BEGIN
	SET NOCOUNT ON;

	select 0 as ResultCode
	
	-- select all shop items
	      SELECT ItemID, Category, IsNew, Price1, Price7, Price30, PriceP, GPrice1, GPrice7, GPrice30, GPriceP FROM Items_Gear
	union SELECT ItemID, Category, IsNew, Price1, Price7, Price30, PriceP, GPrice1, GPrice7, GPrice30, GPriceP	FROM Items_Weapons
	union SELECT ItemID, Category, IsNew, Price1, Price7, Price30, PriceP, GPrice1, GPrice7, GPrice30, GPriceP	FROM Items_Generic
	union SELECT ItemID, Category, IsNew, Price1, Price7, Price30, PriceP, GPrice1, GPrice7, GPrice30, GPriceP	FROM Items_Attachments
	
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_LeaderboardGet]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_LeaderboardGet]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_LeaderboardGet]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_LeaderboardGet]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_LeaderboardGet]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_LeaderboardGet]
	@in_Hardcore int,
	@in_Type int,
	@in_Page int
AS
BEGIN
	SET NOCOUNT ON;

	-- this call is always success
	SELECT 0 AS ResultCode

	DECLARE @rowsPerPage FLOAT = 100.0
	DECLARE @rowLimit INT = 2000;
	DECLARE @startPos INT = (@in_Page - 1) * @rowsPerPage

	IF @in_Type = 5 -- Heroes
	BEGIN
		SELECT TOP (@rowLimit) (@startPos + 1) AS StartPos, 
			CEILING(COUNT(*) / @rowsPerPage) AS PageCount
		FROM UsersChars
		WHERE Hardcore = @in_Hardcore AND Reputation >= 0		

		;WITH LeaderboardData AS
		(
			SELECT TOP (@rowLimit) Gamertag, Alive, Reputation AS Data,
				ROW_NUMBER() OVER
					(ORDER BY Reputation DESC) AS Pos
			FROM UsersChars
			WHERE Hardcore = @in_Hardcore AND Reputation >= 0
		)
		SELECT Gamertag, Alive, Data
		FROM LeaderboardData
		WHERE Pos > @startPos AND Pos <= @startPos + @rowsPerPage
	END
	ELSE IF @in_Type = 6 -- Villains
	BEGIN
		SELECT TOP (@rowLimit) (@startPos + 1) AS StartPos, 
			CEILING(COUNT(*) / @rowsPerPage) AS PageCount
		FROM UsersChars
		WHERE Hardcore = @in_Hardcore AND Reputation < 0	
	
		;WITH LeaderboardData AS
		(
			SELECT TOP (@rowLimit) Gamertag, Alive, Reputation AS Data,
				ROW_NUMBER() OVER
					(ORDER BY Reputation) AS Pos
			FROM UsersChars
			WHERE Hardcore = @in_Hardcore AND Reputation < 0
		)
		SELECT Gamertag, Alive, Data
		FROM LeaderboardData
		WHERE Pos > @startPos AND Pos <= @startPos + @rowsPerPage
	END
	ELSE
	BEGIN
		SELECT TOP (@rowLimit) (@startPos + 1) AS StartPos, 
			CEILING(COUNT(*) / @rowsPerPage) AS PageCount
		FROM UsersChars
		WHERE Hardcore = @in_Hardcore

		;WITH LeaderboardData AS
		(
			SELECT TOP (@rowLimit) Gamertag, Alive,
				CASE @in_Type
					WHEN 0 THEN XP
					WHEN 1 THEN TimePlayed
					WHEN 2 THEN Stat00 -- KilledZombies
					WHEN 3 THEN Stat01 -- KilledSurvivors
					WHEN 4 THEN Stat02 -- KilledBandits
					WHEN 5 THEN Reputation
					WHEN 6 THEN Reputation
					ELSE 0
				END AS Data,
				ROW_NUMBER() OVER
					(ORDER BY
						CASE WHEN @in_Type = 0 THEN XP END DESC,
						CASE WHEN @in_Type = 1 THEN TimePlayed END DESC,
						CASE WHEN @in_Type = 2 THEN Stat00 END DESC,
						CASE WHEN @in_Type = 3 THEN Stat01 END DESC,
						CASE WHEN @in_Type = 4 THEN Stat02 END DESC,
						CASE WHEN @in_Type = 5 THEN Reputation END DESC,
						CASE WHEN @in_Type = 6 THEN Reputation END) AS Pos
			FROM UsersChars
			WHERE Hardcore = @in_Hardcore
		)
		SELECT Gamertag, Alive, Data
		FROM LeaderboardData
		WHERE Pos > @startPos AND Pos <= @startPos + @rowsPerPage
	END
	
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_AddCheatAttempt]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_AddCheatAttempt]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_AddCheatAttempt]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_AddCheatAttempt]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_SRV_AddCheatAttempt]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_SRV_AddCheatAttempt]
	@in_IP char(32),
	@in_CustomerID int,
	@in_GameSessionID bigint,

	@in_CheatID int
AS
BEGIN
	SET NOCOUNT ON;

	INSERT INTO CheatLog (SessionID, CustomerID, CheatID, ReportTime)
	VALUES               (@in_GameSessionID, @in_CustomerID, @in_CheatID, GETDATE())

	-- we're done
	select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_AddLogInfo]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_AddLogInfo]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_AddLogInfo]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_AddLogInfo]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_SRV_AddLogInfo]
-- ----------------------------


CREATE PROCEDURE [dbo].[WZ_SRV_AddLogInfo]
	@in_CustomerID int,
	@in_CharID int = 0,
	@in_Gamertag nvarchar(64) = N'',
	@in_CustomerIP varchar(64),
	@in_GameSessionID bigint,
	@in_CheatID int,
	@in_Msg varchar(4000),
	@in_Data varchar(4000)
AS
BEGIN
	SET NOCOUNT ON;
	
	-- see if this event is recurring inside single game session
	--declare @RecordID int
	--select @RecordID=RecordID from DBG_SrvLogInfo where
	--	GameSessionID=@in_GameSessionID 
	--	and CustomerID=@in_CustomerID
	--	and (@in_CheatID > 0 and CheatID=@in_CheatID)
	--	and @in_Msg=Msg 
	--	and @in_Data=Data
	--if(@@ROWCOUNT > 0) begin
	--	-- increase count
	--	update DBG_SrvLogInfo set RepeatCount=RepeatCount+1 where RecordID=@RecordID
	--	select 0 as ResultCode
	--	return
	--end
	
	insert into DBG_SrvLogInfo (
		ReportTime,
		IsProcessed,
		CustomerID,
		CharID,
		Gamertag,
		CustomerIP,
		GameSessionID,
		CheatID,
		RepeatCount,
		Msg,
		Data)
	values (
		GETDATE(),
		0,
		@in_CustomerID,
		@in_CharID,
		@in_Gamertag,
		@in_CustomerIP,
		@in_GameSessionID,
		@in_CheatID,
		1,
		@in_Msg,
		@in_Data)
		
	-- we're done
	select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_AddWeaponStats]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_AddWeaponStats]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_AddWeaponStats]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_AddWeaponStats]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_SRV_AddWeaponStats]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_SRV_AddWeaponStats] 
	@in_ItemID int,
	@in_ShotsFired int,
	@in_ShotsHits int,
	@in_KillsCQ int,
	@in_KillsDM int,
	@in_KillsSB int
AS
BEGIN
	SET NOCOUNT ON;

	update Items_Weapons set
		ShotsFired=(ShotsFired + @in_ShotsFired),
		ShotsHits=(ShotsHits + @in_ShotsHits),
		KillsCQ=(KillsCQ + @in_KillsCQ),
		KillsDM=(KillsDM + @in_KillsDM),
		KillsSB=(KillsSB + @in_KillsSB)
	where ItemID=@in_ItemID

	-- we're done
	select 0 as ResultCode

END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_NoteAddNew]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_NoteAddNew]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_NoteAddNew]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_NoteAddNew]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_SRV_NoteAddNew]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_SRV_NoteAddNew]
	@in_CustomerID int,
	@in_CharID int,
	@in_GameServerID int,
	@in_GamePos varchar(128),
	@in_ExpireMins int,
	@in_TextFrom nvarchar(128),
	@in_TextSubj nvarchar(2048)
AS
BEGIN
	SET NOCOUNT ON;

	insert into ServerNotesData (
		GameServerId,
		GamePos,
		CreateUtcDate,
		[ExpireUtcDate],
		CustomerID,
		CharID,
		TextFrom,
		TextSubj
	) values (
		@in_GameServerID,
		@in_GamePos,
		GETUTCDATE(),
		DATEADD(mi, @in_ExpireMins, GETUTCDATE()),
		@in_CustomerID,
		@in_CharID,
		@in_TextFrom,
		@in_TextSubj
	)
	declare @NoteID int = SCOPE_IDENTITY()
		
	-- we're done
	select 0 as ResultCode
	select @NoteID as 'NoteID'
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_NoteGetAll]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_NoteGetAll]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_NoteGetAll]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_NoteGetAll]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_SRV_NoteGetAll]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_SRV_NoteGetAll]
	@in_GameServerID int
AS
BEGIN
	SET NOCOUNT ON;

	-- we're done
	select 0 as ResultCode

	delete from ServerNotesData where GameServerId=@in_GameServerID and GETUTCDATE()>[ExpireUtcDate]
	
	select GETUTCDATE() as 'CurUtcDate'
	select * from ServerNotesData where GameServerId=@in_GameServerID
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_SkillAddNew]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_SkillAddNew]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_SkillAddNew]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_SkillAddNew]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_SRV_SkillAddNew]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_SRV_SkillAddNew]
	@in_CharID int,
	@in_SkillID int
AS
BEGIN
	SET NOCOUNT ON;
	declare @XP int 
	
	SELECT @XP=XP FROM UsersChars WHERE CharID=@in_CharID
	
	If(@in_SkillID = 0 AND @XP > 100) begin
		Update UsersChars SET SkilliD0=1, XP=@XP-100 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 1 AND @XP > 110) begin
		Update UsersChars SET SkilliD1=1, XP=@XP-110 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 2 AND @XP > 120) begin
		Update UsersChars SET SkilliD2=1, XP=@XP-120 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 3 AND @XP > 130) begin
		Update UsersChars SET SkilliD3=1, XP=@XP-130 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 4 AND @XP > 140) begin
		Update UsersChars SET SkilliD4=1, XP=@XP-140 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 5 AND @XP > 150) begin
		Update UsersChars SET SkilliD5=1, XP=@XP-150 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 6 AND @XP > 160) begin
		Update UsersChars SET SkilliD6=1, XP=@XP-160 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 7 AND @XP > 170) begin
		Update UsersChars SET SkilliD7=1, XP=@XP-170 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 8 AND @XP > 180) begin
		Update UsersChars SET SkilliD8=1, XP=@XP-180 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 9 AND @XP > 190) begin
		Update UsersChars SET SkilliD9=1, XP=@XP-190 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 10 AND @XP > 200) begin
		Update UsersChars SET SkilliD10=1, XP=@XP-200 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 11 AND @XP > 210) begin
		Update UsersChars SET SkilliD11=1, XP=@XP-210 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 12 AND @XP > 220) begin
		Update UsersChars SET SkilliD12=1, XP=@XP-220 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 13 AND @XP > 230) begin
		Update UsersChars SET SkilliD13=1, XP=@XP-230 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 14 AND @XP > 240) begin
		Update UsersChars SET SkilliD14=1, XP=@XP-240 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 15) begin
		Update UsersChars SET SkilliD15=1 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 16) begin
		Update UsersChars SET SkilliD16=1 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 17) begin
		Update UsersChars SET SkilliD17=1 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 18) begin
		Update UsersChars SET SkilliD18=1 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 19 AND @XP > 290) begin
		Update UsersChars SET SkilliD19=1, XP=@XP-290 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 20 AND @XP > 300) begin
		Update UsersChars SET SkilliD20=1, XP=@XP-300 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 21 AND @XP > 310) begin
		Update UsersChars SET SkilliD21=1, XP=@XP-310 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 22 AND @XP > 320) begin
		Update UsersChars SET SkilliD22=1, XP=@XP-320 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 23 AND @XP > 330) begin
		Update UsersChars SET SkilliD23=1, XP=@XP-330 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 24 AND @XP > 340) begin
		Update UsersChars SET SkilliD24=1, XP=@XP-340 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 25 AND @XP > 350) begin
		Update UsersChars SET SkilliD25=1, XP=@XP-350 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 26 AND @XP > 360) begin
		Update UsersChars SET SkilliD26=1, XP=@XP-360 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 27 AND @XP > 370) begin
		Update UsersChars SET SkilliD27=1, XP=@XP-370 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 28 AND @XP > 380) begin
		Update UsersChars SET SkilliD28=1, XP=@XP-380 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 29 AND @XP > 390) begin
		Update UsersChars SET SkilliD29=1, XP=@XP-390 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 30 AND @XP > 400)begin
		Update UsersChars SET SkilliD30=1, XP=@XP-400 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 31) begin
		Update UsersChars SET SkilliD31=1 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 32) begin
		Update UsersChars SET SkilliD32=1 WHERE CharID=@in_CharID
	end
	else if(@in_SkillID = 33) begin
		Update UsersChars SET SkilliD33=1 WHERE CharID=@in_CharID
	end
	else begin
		select 51 as ResultCode
	end
		
	-- we're done
	select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_UserJoinedGame]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_UserJoinedGame]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_UserJoinedGame]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_UserJoinedGame]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_SRV_UserJoinedGame]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_SRV_UserJoinedGame] 
	@in_CustomerID int,
	@in_CharID int,
	@in_GameMapId int,
	@in_GameServerId bigint,
	@in_GamePos varchar(256)
AS
BEGIN
	SET NOCOUNT ON;
	
	-- store current user server location
	update UsersData set 
		lastgamedate=GETDATE(),
		GameServerId=@in_GameServerId
	where CustomerID=@in_CustomerID
	
	-- per char info
	update UsersChars set 
		GameMapId=@in_GameMapId,
		GameServerId=@in_GameServerId
	where CharID=@in_CharID
		
	-- we're done
	select 0 as ResultCode

END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_UserJoinedGame2]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_UserJoinedGame2]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_UserJoinedGame2]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_UserJoinedGame2]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_SRV_UserJoinedGame2]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_SRV_UserJoinedGame2]
	@in_CustomerID int,
	@in_CharID int,
	@in_GameMapId int,
	@in_GameServerId bigint
AS
BEGIN
	SET NOCOUNT ON;

	-- check if game is still active or 90sec passed from last update (COPYPASTE_GAMECHECK, search for others)
	declare @lastgamedate datetime
	declare @GameServerId int
	select @GameServerId=GameServerId, @lastgamedate=lastgamedate from UsersData where CustomerID=@in_CustomerID
	if(@GameServerId > 0 and DATEDIFF(second, @lastgamedate, GETDATE()) < 90) begin
		select 7 as ResultCode, 'game still active' as ResultMsg
		return
	end
	
	-- store current user server location
	update UsersData set 
		lastgamedate=GETDATE(),
		GameServerId=@in_GameServerId
	where CustomerID=@in_CustomerID
	
	-- per char info
	update UsersChars set 
		GameMapId=@in_GameMapId,
		GameServerId=@in_GameServerId
	where CharID=@in_CharID
		
	-- we're done
	select 0 as ResultCode

END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_UserLeftGame]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_UserLeftGame]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_UserLeftGame]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_SRV_UserLeftGame]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_SRV_UserLeftGame]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_SRV_UserLeftGame]
	@in_CustomerID int,
	@in_CharID int,
	@in_GameMapId int,
	@in_GameServerId bigint,
	@in_TimePlayed int
AS
BEGIN
	SET NOCOUNT ON;
	
	-- store current user server location
	update UsersData set 
		lastgamedate=GETDATE(),
		GameServerId=0,
		TimePlayed=(TimePlayed+@in_TimePlayed)
	where CustomerID=@in_CustomerID
	
	-- update some stats here
	
	-- we're done
	select 0 as ResultCode

END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_UpdateAchievementStatus]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_UpdateAchievementStatus]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_UpdateAchievementStatus]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_UpdateAchievementStatus]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_UpdateAchievementStatus]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_UpdateAchievementStatus]
	@in_CustomerID int,
	@in_AchID int,
	@in_AchValue int,
	@in_AchUnlocked int
AS
BEGIN
	SET NOCOUNT ON;

	if not exists (select * from Achievements where (AchID=@in_AchID and CustomerID=@in_CustomerID))
	begin
		INSERT INTO Achievements(
			CustomerID, 
			AchID, 
			Value, 
			Unlocked
		)
		VALUES (
			@in_CustomerID,
			@in_AchID,
			@in_AchValue,
			@in_AchUnlocked
		)
	end
	else
	begin
		UPDATE Achievements SET 
			Value=@in_AchValue,
			Unlocked=@in_AchUnlocked
		WHERE AchID=@in_AchID and CustomerID=@in_CustomerID
    end

    select 0 as ResultCode
    
    -- check for steamID
    declare @SteamID bigint = 0
	--select @SteamID=SteamID from SteamUserIDMap where CustomerID=@in_CustomerID
	--select @SteamID as 'SteamID'
	
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_UpdateLoginSession]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_UpdateLoginSession]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_UpdateLoginSession]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_UpdateLoginSession]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_UpdateLoginSession]
-- ----------------------------

CREATE PROCEDURE [dbo].[WZ_UpdateLoginSession]
	@in_IP varchar(32),
	@in_CustomerID int,
	@in_SessionID int
AS
BEGIN
	SET NOCOUNT ON;
	
	declare @SessionID int

	-- check if we have record for that user          
	SELECT 
		@SessionID=LoginSessions.SessionID
	FROM LoginSessions
	WHERE CustomerID=@in_CustomerID
	if (@@RowCount = 0) begin
		select 6 as ResultCode
		return
	end

	-- compare session key. if it's different, supplied sesson is invalid	
	if(@in_SessionID <> @SessionID) begin
		select 1 as ResultCode
		return
	end
	
	-- update last ping time
	UPDATE LoginSessions SET 
		LoginSessions.TimeUpdated=GETDATE()
	WHERE CustomerID=@in_CustomerID
	
	select 0 as ResultCode
END






GO

-- ----------------------------
-- Procedure structure for [dbo].[WZ_VITALSTATS_V1]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_VITALSTATS_V1]
-- ----------------------------


-- ----------------------------
-- Procedure structure for [dbo].[WZ_VITALSTATS_V1]
-- ----------------------------

-- ----------------------------
-- Procedure structure for [dbo].[WZ_VITALSTATS_V1]
-- ----------------------------
-- ----------------------------
-- Procedure structure for [WZ_VITALSTATS_V1]
-- ----------------------------


CREATE PROCEDURE [dbo].[WZ_VITALSTATS_V1]
AS
BEGIN

	SET NOCOUNT ON;


declare @today datetime
set @today = GETDATE()

declare @Sales int;
declare @TUsers int;
declare @DAU int;
declare @CCU int;
declare @Revenues int;

set @Sales = 0--(select count(*) from [BreezeNet.WarZPreorders])
set @Revenues = 0--(select COALESCE(SUM(Amount),0) from [BreezeNet.WarZPreorders] where OrderDate>=@today and OrderDate<DATEADD(hour, -1, @today))

set @TUsers = (select count(*) from Accounts)
set @DAU = (select count(*) from Accounts where lastlogindate > DATEADD(hour, -24, @today))
set @CCU = (select count(*) from LoginSessions where TimeUpdated > DATEADD(minute, -7, @today))

INSERT INTO VitalStats_V1 VALUES (@today, @Sales, @TUsers, @DAU, @CCU, @Revenues );

	    
END






GO

-- ----------------------------
-- Function structure for [dbo].[fn_diagramobjects]
-- ----------------------------

-- ----------------------------
-- Function structure for [dbo].[fn_diagramobjects]
-- ----------------------------


-- ----------------------------
-- Function structure for [dbo].[fn_diagramobjects]
-- ----------------------------

-- ----------------------------
-- Function structure for [dbo].[fn_diagramobjects]
-- ----------------------------
-- ----------------------------
-- Function structure for [fn_diagramobjects]
-- ----------------------------

	CREATE FUNCTION [dbo].[fn_diagramobjects]() 
	RETURNS int
	WITH EXECUTE AS N'dbo'
	AS
	BEGIN
		declare @id_upgraddiagrams		int
		declare @id_sysdiagrams			int
		declare @id_helpdiagrams		int
		declare @id_helpdiagramdefinition	int
		declare @id_creatediagram	int
		declare @id_renamediagram	int
		declare @id_alterdiagram 	int 
		declare @id_dropdiagram		int
		declare @InstalledObjects	int

		select @InstalledObjects = 0

		select 	@id_upgraddiagrams = object_id(N'dbo.sp_upgraddiagrams'),
			@id_sysdiagrams = object_id(N'dbo.sysdiagrams'),
			@id_helpdiagrams = object_id(N'dbo.sp_helpdiagrams'),
			@id_helpdiagramdefinition = object_id(N'dbo.sp_helpdiagramdefinition'),
			@id_creatediagram = object_id(N'dbo.sp_creatediagram'),
			@id_renamediagram = object_id(N'dbo.sp_renamediagram'),
			@id_alterdiagram = object_id(N'dbo.sp_alterdiagram'), 
			@id_dropdiagram = object_id(N'dbo.sp_dropdiagram')

		if @id_upgraddiagrams is not null
			select @InstalledObjects = @InstalledObjects + 1
		if @id_sysdiagrams is not null
			select @InstalledObjects = @InstalledObjects + 2
		if @id_helpdiagrams is not null
			select @InstalledObjects = @InstalledObjects + 4
		if @id_helpdiagramdefinition is not null
			select @InstalledObjects = @InstalledObjects + 8
		if @id_creatediagram is not null
			select @InstalledObjects = @InstalledObjects + 16
		if @id_renamediagram is not null
			select @InstalledObjects = @InstalledObjects + 32
		if @id_alterdiagram  is not null
			select @InstalledObjects = @InstalledObjects + 64
		if @id_dropdiagram is not null
			select @InstalledObjects = @InstalledObjects + 128
		
		return @InstalledObjects 
	END






GO

-- ----------------------------
-- Indexes structure for table Accounts
-- ----------------------------
CREATE UNIQUE INDEX [IX_Accounts_email] ON [dbo].[Accounts]
([email] ASC) 
GO

-- ----------------------------
-- Primary Key structure for table [dbo].[Accounts]
-- ----------------------------
ALTER TABLE [dbo].[Accounts] ADD PRIMARY KEY ([CustomerID])
GO

-- ----------------------------
-- Indexes structure for table Achievements
-- ----------------------------
CREATE INDEX [IX_Achievements_AchID] ON [dbo].[Achievements]
([CustomerID] ASC) 
GO
CREATE INDEX [IX_Achievements_CustomerID] ON [dbo].[Achievements]
([CustomerID] ASC) 
GO

-- ----------------------------
-- Indexes structure for table Application_Revive
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[Application_Revive]
-- ----------------------------
ALTER TABLE [dbo].[Application_Revive] ADD PRIMARY KEY ([CustomerID])
GO

-- ----------------------------
-- Indexes structure for table CharsCreateLog
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[CharsCreateLog]
-- ----------------------------
ALTER TABLE [dbo].[CharsCreateLog] ADD PRIMARY KEY ([id])
GO

-- ----------------------------
-- Indexes structure for table CharsLog
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[CharsLog]
-- ----------------------------
ALTER TABLE [dbo].[CharsLog] ADD PRIMARY KEY ([id])
GO

-- ----------------------------
-- Indexes structure for table CharsStats
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[CharsStats]
-- ----------------------------
ALTER TABLE [dbo].[CharsStats] ADD PRIMARY KEY ([CharID])
GO

-- ----------------------------
-- Indexes structure for table CheatLog
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[CheatLog]
-- ----------------------------
ALTER TABLE [dbo].[CheatLog] ADD PRIMARY KEY ([ID])
GO

-- ----------------------------
-- Indexes structure for table ClanApplications
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[ClanApplications]
-- ----------------------------
ALTER TABLE [dbo].[ClanApplications] ADD PRIMARY KEY ([ClanApplicationID])
GO

-- ----------------------------
-- Indexes structure for table ClanData
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[ClanData]
-- ----------------------------
ALTER TABLE [dbo].[ClanData] ADD PRIMARY KEY ([ClanID])
GO

-- ----------------------------
-- Indexes structure for table ClanEvents
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[ClanEvents]
-- ----------------------------
ALTER TABLE [dbo].[ClanEvents] ADD PRIMARY KEY ([ClanEventID])
GO

-- ----------------------------
-- Indexes structure for table ClanInvites
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[ClanInvites]
-- ----------------------------
ALTER TABLE [dbo].[ClanInvites] ADD PRIMARY KEY ([ClanInviteID])
GO

-- ----------------------------
-- Indexes structure for table CRCBanList
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[CRCBanList]
-- ----------------------------
ALTER TABLE [dbo].[CRCBanList] ADD PRIMARY KEY ([CRC])
GO

-- ----------------------------
-- Indexes structure for table DataGameRewards
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[DataGameRewards]
-- ----------------------------
ALTER TABLE [dbo].[DataGameRewards] ADD PRIMARY KEY ([ID])
GO

-- ----------------------------
-- Indexes structure for table DataSkill2Price
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[DataSkill2Price]
-- ----------------------------
ALTER TABLE [dbo].[DataSkill2Price] ADD PRIMARY KEY ([SkillID])
GO

-- ----------------------------
-- Indexes structure for table DBG_GPTransactions
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[DBG_GPTransactions]
-- ----------------------------
ALTER TABLE [dbo].[DBG_GPTransactions] ADD PRIMARY KEY ([TransactionID])
GO

-- ----------------------------
-- Indexes structure for table DBG_IISApiStats
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[DBG_IISApiStats]
-- ----------------------------
ALTER TABLE [dbo].[DBG_IISApiStats] ADD PRIMARY KEY ([API])
GO

-- ----------------------------
-- Indexes structure for table DBG_LevelUpEvents
-- ----------------------------
CREATE INDEX [IX_DBG_LevelUpEvents] ON [dbo].[DBG_LevelUpEvents]
([CustomerID] ASC) 
GO

-- ----------------------------
-- Indexes structure for table DBG_PasswordResets
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[DBG_PasswordResets]
-- ----------------------------
ALTER TABLE [dbo].[DBG_PasswordResets] ADD PRIMARY KEY ([ResetID])
GO

-- ----------------------------
-- Indexes structure for table DBG_UserRoundResults
-- ----------------------------
CREATE INDEX [DBG_URR_CID_GRP] ON [dbo].[DBG_UserRoundResults]
([CustomerID] ASC, [GameReportTime] ASC) 
GO
CREATE INDEX [IX_DBG_UserRoundResults_LBIdx1] ON [dbo].[DBG_UserRoundResults]
([GameReportTime] ASC) 
INCLUDE ([CustomerID], [Deaths], [HonorPoints], [Kills], [Losses], [ShotsFired], [ShotsHits], [TimePlayed], [Wins]) 
GO

-- ----------------------------
-- Indexes structure for table DonateLogs
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[DonateLogs]
-- ----------------------------
ALTER TABLE [dbo].[DonateLogs] ADD PRIMARY KEY ([id])
GO

-- ----------------------------
-- Indexes structure for table FriendsMap
-- ----------------------------
CREATE INDEX [IDX_FriendsMap_CustomerID] ON [dbo].[FriendsMap]
([CustomerID] ASC) 
GO
CREATE INDEX [IDX_FriendsMap_FriendID] ON [dbo].[FriendsMap]
([FriendID] ASC) 
GO

-- ----------------------------
-- Indexes structure for table HWID_Ban
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[HWID_Ban]
-- ----------------------------
ALTER TABLE [dbo].[HWID_Ban] ADD PRIMARY KEY ([HardwareID])
GO

-- ----------------------------
-- Indexes structure for table HWID_Log
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[HWID_Log]
-- ----------------------------
ALTER TABLE [dbo].[HWID_Log] ADD PRIMARY KEY ([id])
GO

-- ----------------------------
-- Indexes structure for table Items_Attachments
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[Items_Attachments]
-- ----------------------------
ALTER TABLE [dbo].[Items_Attachments] ADD PRIMARY KEY ([ItemID])
GO

-- ----------------------------
-- Indexes structure for table Items_Gear
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[Items_Gear]
-- ----------------------------
ALTER TABLE [dbo].[Items_Gear] ADD PRIMARY KEY ([ItemID])
GO

-- ----------------------------
-- Indexes structure for table Items_Generic
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[Items_Generic]
-- ----------------------------
ALTER TABLE [dbo].[Items_Generic] ADD PRIMARY KEY ([ItemID])
GO

-- ----------------------------
-- Indexes structure for table Items_Weapons
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[Items_Weapons]
-- ----------------------------
ALTER TABLE [dbo].[Items_Weapons] ADD PRIMARY KEY ([ItemID])
GO

-- ----------------------------
-- Indexes structure for table Leaderboard
-- ----------------------------
CREATE UNIQUE INDEX [IX_Leaderboard_CustomerID] ON [dbo].[Leaderboard]
([CustomerID] ASC) 
GO

-- ----------------------------
-- Primary Key structure for table [dbo].[Leaderboard]
-- ----------------------------
ALTER TABLE [dbo].[Leaderboard] ADD PRIMARY KEY ([Pos])
GO

-- ----------------------------
-- Indexes structure for table Leaderboard1
-- ----------------------------
CREATE UNIQUE INDEX [IX_Leaderboard1_CustomerID] ON [dbo].[Leaderboard1]
([CustomerID] ASC) 
GO

-- ----------------------------
-- Primary Key structure for table [dbo].[Leaderboard1]
-- ----------------------------
ALTER TABLE [dbo].[Leaderboard1] ADD PRIMARY KEY ([Pos])
GO

-- ----------------------------
-- Indexes structure for table Leaderboard30
-- ----------------------------
CREATE UNIQUE INDEX [IX_Leaderboard30_CustomerID] ON [dbo].[Leaderboard30]
([CustomerID] ASC) 
GO

-- ----------------------------
-- Primary Key structure for table [dbo].[Leaderboard30]
-- ----------------------------
ALTER TABLE [dbo].[Leaderboard30] ADD PRIMARY KEY ([Pos])
GO

-- ----------------------------
-- Indexes structure for table Leaderboard7
-- ----------------------------
CREATE UNIQUE INDEX [IX_Leaderboard7_CustomerID] ON [dbo].[Leaderboard7]
([CustomerID] ASC) 
GO

-- ----------------------------
-- Primary Key structure for table [dbo].[Leaderboard7]
-- ----------------------------
ALTER TABLE [dbo].[Leaderboard7] ADD PRIMARY KEY ([Pos])
GO

-- ----------------------------
-- Indexes structure for table LoginLog
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[LoginLog]
-- ----------------------------
ALTER TABLE [dbo].[LoginLog] ADD PRIMARY KEY ([id])
GO

-- ----------------------------
-- Indexes structure for table Logins
-- ----------------------------
CREATE INDEX [IX_Logins_CustomerID_LoginTime] ON [dbo].[Logins]
([CustomerID] ASC, [LoginTime] ASC) 
GO

-- ----------------------------
-- Primary Key structure for table [dbo].[Logins]
-- ----------------------------
ALTER TABLE [dbo].[Logins] ADD PRIMARY KEY ([LoginID])
GO

-- ----------------------------
-- Indexes structure for table LoginSessions
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[LoginSessions]
-- ----------------------------
ALTER TABLE [dbo].[LoginSessions] ADD PRIMARY KEY ([CustomerID])
GO

-- ----------------------------
-- Indexes structure for table MacBan
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[MacBan]
-- ----------------------------
ALTER TABLE [dbo].[MacBan] ADD PRIMARY KEY ([Mac])
GO

-- ----------------------------
-- Indexes structure for table MasterServerInfo
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[MasterServerInfo]
-- ----------------------------
ALTER TABLE [dbo].[MasterServerInfo] ADD PRIMARY KEY ([ServerID])
GO

-- ----------------------------
-- Indexes structure for table PremiumReceiveGCLog
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[PremiumReceiveGCLog]
-- ----------------------------
ALTER TABLE [dbo].[PremiumReceiveGCLog] ADD PRIMARY KEY ([Receive])
GO

-- ----------------------------
-- Indexes structure for table SecurityLog
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[SecurityLog]
-- ----------------------------
ALTER TABLE [dbo].[SecurityLog] ADD PRIMARY KEY ([ID])
GO

-- ----------------------------
-- Indexes structure for table ServerNotesData
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[ServerNotesData]
-- ----------------------------
ALTER TABLE [dbo].[ServerNotesData] ADD PRIMARY KEY ([NoteID])
GO

-- ----------------------------
-- Indexes structure for table UsersChars
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[UsersChars]
-- ----------------------------
ALTER TABLE [dbo].[UsersChars] ADD PRIMARY KEY ([CharID])
GO

-- ----------------------------
-- Indexes structure for table UsersData
-- ----------------------------
CREATE INDEX [IX_LoginID_ClanID] ON [dbo].[UsersData]
([ClanID] ASC) 
GO

-- ----------------------------
-- Primary Key structure for table [dbo].[UsersData]
-- ----------------------------
ALTER TABLE [dbo].[UsersData] ADD PRIMARY KEY ([CustomerID])
GO

-- ----------------------------
-- Indexes structure for table UsersInventory
-- ----------------------------
CREATE INDEX [IX_Inventory_CustomerID] ON [dbo].[UsersInventory]
([CustomerID] ASC) 
GO
CREATE INDEX [IX_UsersInventory_CharID] ON [dbo].[UsersInventory]
([CharID] ASC) 
GO

-- ----------------------------
-- Primary Key structure for table [dbo].[UsersInventory]
-- ----------------------------
ALTER TABLE [dbo].[UsersInventory] ADD PRIMARY KEY ([InventoryID])
GO

-- ----------------------------
-- Indexes structure for table UsersPrivate
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[UsersPrivate]
-- ----------------------------
ALTER TABLE [dbo].[UsersPrivate] ADD PRIMARY KEY ([CustomerID])
GO

-- ----------------------------
-- Indexes structure for table WebLoginSessions
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table [dbo].[WebLoginSessions]
-- ----------------------------
ALTER TABLE [dbo].[WebLoginSessions] ADD PRIMARY KEY ([CustomerID])
GO
