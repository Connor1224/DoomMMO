﻿using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Data;
using System.Text;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Data.SqlClient;
using System.Configuration;

public partial class Web_webapi_ApplicationRevive : WOApiWebPage
{
    protected override void Execute()
    {
        string CustomerID = web.CustomerID();
        string Point = web.Param("point");

        SqlCommand sqcmd = new SqlCommand();
        sqcmd.CommandType = CommandType.StoredProcedure;
        sqcmd.CommandText = "WEB_Application_Revive";
        sqcmd.Parameters.AddWithValue("@in_CustomerID", CustomerID);
        sqcmd.Parameters.AddWithValue("@in_Point", Point);

        if (!CallWOApi(sqcmd))
            return;

        Response.Write("WO_0");

    }
}
