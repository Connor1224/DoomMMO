using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Data.SqlClient;
using System.Data;

public partial class api_BanPlayer : WOApiWebPage
{
    protected override void Execute()
    {
        if (!WoCheckLoginSession())
            return;
        string skey1 = web.Param("skey1");
        if (skey1 != SERVER_API_KEY)
            throw new ApiExitException("bad key");


        string CustomerID = web.CustomerID();

        SqlCommand sqcmd = new SqlCommand();
        sqcmd.CommandType = CommandType.StoredProcedure;
        sqcmd.CommandText = "BanPlayer";
        sqcmd.Parameters.AddWithValue("@in_CustomerID", CustomerID);
        sqcmd.Parameters.AddWithValue("@in_BanID", web.Param("BanID"));
        sqcmd.Parameters.AddWithValue("@in_Reason", web.Param("Reason"));

        if (!CallWOApi(sqcmd))
            return;
        Response.Write("WO_0");
    }
}