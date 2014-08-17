using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Data;
using System.Text;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Data.SqlClient;
using System.Configuration;

public partial class api_GetItemQuantityBuy : WOApiWebPage
{
    protected override void Execute()
    {
        //if (!WoCheckLoginSession())
        //    return;

        int ItemID = Convert.ToInt32(web.Param("ItemID"));

        SqlCommand sqcmd = new SqlCommand();
        sqcmd.CommandType = CommandType.StoredProcedure;
        sqcmd.CommandText = "FN_GetItemQuantityBuy";
        sqcmd.Parameters.AddWithValue("@in_ItemID", ItemID);

        if (!CallWOApi(sqcmd))
            return;

        //reader.Read();

        // report page of leaderboard
        StringBuilder xml = new StringBuilder();
        xml.Append("<?xml version=\"1.0\"?>\n");

        //reader.NextResult();
        while (reader.Read())
        {
            string Clipsize = reader["Clipsize"].ToString();;
            
            xml.Append("<Items_Weapons ");
            xml.Append(xml_attr("Clipsize",Clipsize));
            xml.Append(" />");
        }

        Response.Write(xml.ToString());
    }
}