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

public partial class api_GetTransactions : WOApiWebPage
{
    protected override void Execute()
    {
        //if (!WoCheckLoginSession())
        //    return;

        string CustomerID = web.Param("CustomerID");

        SqlCommand sqcmd = new SqlCommand();
        sqcmd.CommandType = CommandType.StoredProcedure;
        sqcmd.CommandText = "WZ_GetTransactions";
        sqcmd.Parameters.AddWithValue("@in_CustomerID", CustomerID);

        if (!CallWOApi(sqcmd))
            return;

        //reader.Read();

        // report page of leaderboard
        StringBuilder xml = new StringBuilder();
        xml.Append("<?xml version=\"1.0\"?>\n");

        //reader.NextResult();
        while (reader.Read())
        {
            string DateTime = reader["DateTime"].ToString();
            string TransactionID = reader["TransactionID"].ToString();
            string ItemID = reader["ItemID"].ToString();
            string Amount = reader["Amount"].ToString();
            
            xml.Append("<data ");
            xml.Append(xml_attr("DateTime",DateTime));
            xml.Append(xml_attr("TransactionID",TransactionID));
            xml.Append(xml_attr("ItemID",ItemID));
            xml.Append(xml_attr("Amount",Amount));
            xml.Append(" />");
        }

        Response.Write(xml.ToString());
    }
}