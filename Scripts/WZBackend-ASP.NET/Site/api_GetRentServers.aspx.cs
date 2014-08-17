using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Data;
using System.Text;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Data.SqlClient;
using System.IO;

public partial class api_GetRentServers : WOApiWebPage
{

    protected override void Execute()
    {
        SqlCommand sqcmd = new SqlCommand();
        sqcmd.CommandType = CommandType.StoredProcedure;
        sqcmd.CommandText = "WZ_GetRentServers";

        if (!CallWOApi(sqcmd))
            return;

        StringBuilder xml = new StringBuilder();
        xml.Append("<?xml version=\"1.0\"?>\n");

        while (reader.Read())
        {
            
            string gameServerId = reader["gameServerId"].ToString();
	        string MapSettings = reader["MapSettings"].ToString();
	        string Region = reader["Region"].ToString();
	        string Name = reader["Name"].ToString();
	        string MapID = reader["MapID"].ToString();
	        string Password = reader["Password"].ToString();
	        string MaxPlayers = reader["MaxPlayers"].ToString();
	        string Owner = reader["Owner"].ToString();
	        string ExpireTime = reader["ExpireTime"].ToString();
	        string RentalTime = reader["RentalTime"].ToString();
            string Crosshair = reader["Crosshair"].ToString();
            string SNP = reader["SNP"].ToString();

            xml.Append("<Rent_Servers ");
            xml.Append(xml_attr("gameServerId",gameServerId));
            xml.Append(xml_attr("MapSettings",MapSettings));
            xml.Append(xml_attr("Region",Region));
            xml.Append(xml_attr("Name",Name));
            xml.Append(xml_attr("MapID",MapID));
            xml.Append(xml_attr("Password",Password));
            xml.Append(xml_attr("MaxPlayers",MaxPlayers));
            xml.Append(xml_attr("Owner",Owner));
            xml.Append(xml_attr("ExpireTime",ExpireTime));
            xml.Append(xml_attr("RentalTime",RentalTime));
            xml.Append(xml_attr("Crosshair", Crosshair));
            xml.Append(xml_attr("SNP", SNP));
            xml.Append("/>\n");
        }

        GResponse.Write(xml.ToString());

    }
}
