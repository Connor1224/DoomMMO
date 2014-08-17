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
using System.Text.RegularExpressions;

public partial class api_SrvRentGame : WOApiWebPage
{
    void OutfitOp(string StateSesion, string gameServerId, string MapSettings, string Region, string Name, string MapID, string Password, string MaxPlayers, string Owner, string ExpireTime, string RentalTime, string Crosshair, string SNP)
    {
        int StateSesion1 = Convert.ToInt32(StateSesion);
        int gameServerId1 = Convert.ToInt32(gameServerId);
        int MapSettings1 = Convert.ToInt32(MapSettings);
        int Region1 = Convert.ToInt32(Region);
        int MapID1 = Convert.ToInt32(MapID);
        int MaxPlayers1 = Convert.ToInt32(MaxPlayers);
        int Owner1 = Convert.ToInt32(Owner);
        int ExpireTime1 = Convert.ToInt32(ExpireTime);
        int RentalTime1 = Convert.ToInt32(RentalTime);
        int Crosshair1 = Convert.ToInt32(Crosshair);
        int SNP1 = Convert.ToInt32(SNP);

        SqlCommand sqcmd = new SqlCommand();
        sqcmd.CommandType = CommandType.StoredProcedure;
        sqcmd.CommandText = "WZ_SRV_AddRentGame";
        sqcmd.Parameters.AddWithValue("@in_StateSesion", StateSesion1);
        sqcmd.Parameters.AddWithValue("@in_gameServerId", gameServerId1);
        sqcmd.Parameters.AddWithValue("@in_MapSettings", MapSettings1);
        sqcmd.Parameters.AddWithValue("@in_Region", Region1);
        sqcmd.Parameters.AddWithValue("@in_Name", Name);
        sqcmd.Parameters.AddWithValue("@in_MapID", MapID1);
        sqcmd.Parameters.AddWithValue("@in_Password", Password);
        sqcmd.Parameters.AddWithValue("@in_MaxPlayers", MaxPlayers1);
        sqcmd.Parameters.AddWithValue("@in_Owner", Owner1);
        sqcmd.Parameters.AddWithValue("@in_ExpireTime", ExpireTime1);
        sqcmd.Parameters.AddWithValue("@in_RentalTime", RentalTime1);
        sqcmd.Parameters.AddWithValue("@in_Crosshair", Crosshair1);
        sqcmd.Parameters.AddWithValue("@in_SNP", SNP1);

        if (!CallWOApi(sqcmd))
            return;

        Response.Write("WO_0");
    }

    protected override void Execute()
    {
        if (!WoCheckLoginSession())
            return;

        //         string func = web.Param("func");
        string StateSesion = web.Param("StateSesion");
        string gameServerId = web.Param("gameServerId");
	    string MapSettings = web.Param("MapSettings");
	    string Region = web.Param("Region");
	    string Name = web.Param("Name");
	    string MapID = web.Param("MapID");
	    string Password = web.Param("Password");
	    string MaxPlayers = web.Param("MaxPlayers");
	    string Owner = web.Param("Owner");
	    string ExpireTime = web.Param("ExpireTime");
	    string RentalTime = web.Param("RentalTime");
        string Crosshair = web.Param("Crosshair");
        string SNP = web.Param("SNP");

        if (Password.IndexOfAny("!@#$%^&*()-=+_<>,./?'\":;|{}[]".ToCharArray()) >= 0) // do not allow this symbols
        {
            Response.Write("WO_7");
            Response.Write("the Password cannot contain special symbols");
            return;
        }

        OutfitOp(StateSesion, gameServerId, MapSettings, Region, Name, MapID, Password, MaxPlayers, Owner, ExpireTime, RentalTime, Crosshair,SNP);
    }
}