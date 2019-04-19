
#include "makeHists.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TFile.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TMath.h"
  
using namespace std;
void makeHists::bookHists(const char* fName)
{
	hf = new TFile(fName,"RECREATE");
	char name[6][30] = {"ZSelection","relIsoSelection","chargeSelection","HighestPtSelection","MllDiffChargeSelection","nbSelection"};
	char namegood[50];
	char namefake[50];
	char namerest[50];



	char plotname[100];

	char name1[3][10] = {"_em","_onZ","_offZ"};
	char name2[8][20] = {"_1stPt","_2stPt","_3stPt","_4stPt","_1stPtFromZ","_2stPtFromZ","_1stPtNotfromZ","_2stPtNotfromZ"};
	for(int cir1 = 0 ; cir1 < 3 ; cir1++){
		sprintf(plotname,"%s%s","plot_lepton_Mll", name1[cir1] );
		plot_lepton_Mll[cir1] = new TH1D(plotname,plotname,300,0,300);
		sprintf(plotname,"%s%s","plot_lepton_missingET",name1[cir1]);
		plot_lepton_missingET[cir1] = new TH1D(plotname,plotname,300,0,300);
		sprintf(plotname,"%s%s","plot_nj",name1[cir1]);
		plot_nj[cir1] = new TH1D(plotname,plotname,20,0,20);		
		sprintf(plotname,"%s%s","plot_ht",name1[cir1]);
		plot_ht[cir1] = new TH1D(plotname,plotname,300,0,300);
		sprintf(plotname,"%s%s","plot_St",name1[cir1]);
		plot_St[cir1] = new TH1D(plotname,plotname,300,0,300);
		sprintf(plotname,"%s%s","plot_Mjet_DeltaR",name1[cir1]);
		plot_Mjet_DeltaR[cir1] = new TH1D(plotname,plotname,100,0,100);
		sprintf(plotname,"%s%s","plot_Mjet_LeadPt",name1[cir1]);
		plot_Mjet_LeadPt[cir1] = new TH1D(plotname,plotname,300,0,300);

		for(int cir2 = 0 ; cir2 < 8 ; cir2 ++){
			sprintf(plotname,"%s%s%s","plot_lepton_Pt",name1[cir1],name2[cir2]);
			plot_lepton_Pt[cir1][cir2] = new TH1D(plotname,plotname,300,0,300);
			sprintf(plotname,"%s%s%s","plot_lepton_Eta",name1[cir1],name2[cir2]);
			plot_lepton_Eta[cir1][cir2] = new TH1D(plotname,plotname,200,-4,4);
			sprintf(plotname,"%s%s%s","plot_lepton_relIsoEA",name1[cir1],name2[cir2]);
			plot_lepton_relIsoEA[cir1][cir2] = new TH1D(plotname,plotname,200,0,1);
			sprintf(plotname,"%s%s%s","plot_lepton_sip3d",name1[cir1],name2[cir2]);
			plot_lepton_sip3d[cir1][cir2] = new TH1D(plotname,plotname,200,-10,10);
			sprintf(plotname,"%s%s%s","plot_lepton_ip",name1[cir1],name2[cir2]);
			plot_lepton_ip[cir1][cir2] = new TH1D(plotname,plotname,200,-10,10);
			sprintf(plotname,"%s%s%s","plot_lepton_dxy",name1[cir1],name2[cir2]);
			plot_lepton_dxy[cir1][cir2] = new TH1D(plotname,plotname,200,-10,10);
			sprintf(plotname,"%s%s%s","plot_lepton_dz",name1[cir1],name2[cir2]);
			plot_lepton_dz[cir1][cir2] = new TH1D(plotname,plotname,200,-10,10);
			sprintf(plotname,"%s%s%s","plot_lepton_mva",name1[cir1],name2[cir2]);
			plot_lepton_mva[cir1][cir2] = new TH1D(plotname,plotname,200,-10,10);
		}
	}


	for(int cir1 = 0 ; cir1 < 6 ; cir1++){
		sprintf(namegood,"%s%s","plot_dilep_good_Mll_",name[cir1]);
		sprintf(namefake,"%s%s","plot_dilep_fake_Mll_",name[cir1]);
		sprintf(namerest,"%s%s","plot_dilep_rest_Mll_",name[cir1]);
		plot_dilep_good_Mll[cir1] = new TH1D(namegood,namegood,160,40,200);
		plot_dilep_fake_Mll[cir1] = new TH1D(namefake,namefake,160,40,200);
		plot_dilep_rest_Mll[cir1] = new TH1D(namerest,namerest,160,40,200);
		plot_dilep_good_Mll[cir1]->Sumw2();
		plot_dilep_fake_Mll[cir1]->Sumw2();
		plot_dilep_rest_Mll[cir1]->Sumw2();
	}

	plot_dilep_good_relIso = new TH1D("rel_good","rel_good",10,0,0.4);
	plot_dilep_fake_relIso = new TH1D("rel_fake","rel_fake",10,0,0.4);

	plot_dilep_good_Mll_em   = new TH1D("plot_dilep_good_Mll_em"  ,"plot_dilep_good_Mll_em"  ,160,40,200);
	plot_dilep_good_Mll_onZ  = new TH1D("plot_dilep_good_Mll_onZ" ,"plot_dilep_good_Mll_onZ" ,160,40,200);
	plot_dilep_good_Mll_offZ = new TH1D("plot_dilep_good_Mll_offZ","plot_dilep_good_Mll_offZ",160,40,200);
	plot_dilep_fake_Mll_em   = new TH1D("plot_dilep_fake_Mll_em"  ,"plot_dilep_fake_Mll_em"  ,160,40,200);
	plot_dilep_fake_Mll_onZ  = new TH1D("plot_dilep_fake_Mll_onZ" ,"plot_dilep_fake_Mll_onZ" ,160,40,200);
	plot_dilep_fake_Mll_offZ = new TH1D("plot_dilep_fake_Mll_offZ","plot_dilep_fake_Mll_offZ",160,40,200);
	plot_dilep_rest_Mll_em   = new TH1D("plot_dilep_rest_Mll_em"  ,"plot_dilep_rest_Mll_em"  ,160,40,200);
	plot_dilep_rest_Mll_onZ  = new TH1D("plot_dilep_rest_Mll_onZ" ,"plot_dilep_rest_Mll_onZ" ,160,40,200);
	plot_dilep_rest_Mll_offZ = new TH1D("plot_dilep_rest_Mll_offZ","plot_dilep_rest_Mll_offZ",160,40,200);

	plot_dilep_good_Mll_em   -> Sumw2();
	plot_dilep_good_Mll_onZ  -> Sumw2();
	plot_dilep_good_Mll_offZ -> Sumw2();
	plot_dilep_fake_Mll_em   -> Sumw2();
	plot_dilep_fake_Mll_onZ  -> Sumw2();
	plot_dilep_fake_Mll_offZ -> Sumw2();
	plot_dilep_rest_Mll_em   -> Sumw2();
	plot_dilep_rest_Mll_onZ  -> Sumw2();
	plot_dilep_rest_Mll_offZ -> Sumw2();
	plot_dilep_good_relIso -> Sumw2();
	plot_dilep_fake_relIso -> Sumw2();



//	char nameeta[100];
//	for(int cir1 = 0 ; cir1 < 12 ; cir1 ++){
//		sprintf(nameeta,"%s%d","plot_zboson_mass_vs_muon_eta",cir1);
//		plot_zboson_mass_vs_muon_eta[cir1] = new TH1D(nameeta,nameeta,140,60,130);
//		sprintf(nameeta,"%s%d","plot_zboson_mass_vs_antimuon_eta",cir1);
//		plot_zboson_mass_vs_antimuon_eta[cir1] = new TH1D(nameeta,nameeta,140,60,130);
//		plot_zboson_mass_vs_muon_eta[cir1]->Sumw2();
//		plot_zboson_mass_vs_antimuon_eta[cir1]->Sumw2();
//	}

//        plot_ee_lep_pt      = new TH1D("plot_ee_lep_pt",    "plot_ee_lep_pt",100,0,400);
//        plot_ee_lep_pt->Sumw2();
//        plot_em_lep_pt      = new TH1D("plot_em_lep_pt",    "plot_em_lep_pt",100,0,400);
//        plot_em_lep_pt->Sumw2();
//        plot_mm_lep_pt      = new TH1D("plot_mm_lep_pt",    "plot_mm_lep_pt",100,0,400);
//        plot_mm_lep_pt->Sumw2();
//        plot_ee_lep_eta     = new TH1D("plot_ee_lep_eta",   "plot_ee_lep_eta",100,-5,5);
//        plot_ee_lep_eta->Sumw2();
//        plot_em_lep_eta     = new TH1D("plot_em_lep_eta",   "plot_em_lep_eta",100,-5,5);
//        plot_em_lep_eta->Sumw2();
//        plot_mm_lep_eta     = new TH1D("plot_mm_lep_eta",   "plot_mm_lep_eta",100,-5,5);
//        plot_mm_lep_eta->Sumw2();
//        plot_ee_lep_trk_pt  = new TH1D("plot_ee_lep_trk_pt","plot_ee_lep_trk_pt",100,0,400);
//        plot_ee_lep_trk_pt->Sumw2();
//        plot_em_lep_trk_pt  = new TH1D("plot_em_lep_trk_pt","plot_em_lep_trk_pt",100,0,400);
//        plot_em_lep_trk_pt->Sumw2();
//        plot_mm_lep_trk_pt  = new TH1D("plot_mm_lep_trk_pt","plot_mm_lep_trk_pt",100,0,400);
//        plot_mm_lep_trk_pt->Sumw2();
//        plot_ee_lep_charge  = new TH1D("plot_ee_lep_charge","plot_ee_lep_charge",10,-1.0,1);
//        plot_ee_lep_charge->Sumw2();
 //       plot_em_lep_charge  = new TH1D("plot_em_lep_charge","plot_em_lep_charge",10,-1.0,1);
//        plot_em_lep_charge->Sumw2();
//        plot_mm_lep_charge  = new TH1D("plot_mm_lep_charge","plot_mm_lep_charge",10,-1.0,1);
//        plot_mm_lep_charge->Sumw2();
/*	plot_zboson_mass_forward = new TH1D("plot_zboson_mass_forward","zboson mass forward",140,60,130);
	plot_zboson_mass_forward->Sumw2();
	plot_zboson_mass_backward = new TH1D("plot_zboson_mass_backward","zboson mass backward",140,60,130);
	plot_zboson_mass_backward->Sumw2();
	char name_seed_muon[100];
	char name_seed_antimuon[100];
	char name_forward_muon[100];
	char name_forward_antimuon[100];
	for(int iplot = 0 ; iplot < 16 ; iplot++)
	{
		sprintf(name_seed_muon, "%s%d", "plot_zboson_mass_seed_muon", iplot);
		sprintf(name_seed_antimuon, "%s%d", "plot_zboson_mass_seed_antimuon", iplot);
		plot_zboson_mass_seed_muon[iplot] = new TH1D(name_seed_muon,name_seed_muon,140,60,130);
		plot_zboson_mass_seed_antimuon[iplot] = new TH1D(name_seed_antimuon,name_seed_antimuon,140,60,130);
		plot_zboson_mass_seed_muon[iplot]->Sumw2();
		plot_zboson_mass_seed_antimuon[iplot]->Sumw2();
	}
	for(int iplot = 0; iplot<16;iplot++)
	{
		sprintf(name_forward_muon,"%s%d","plot_zboson_mass_forward_muon",iplot);
		sprintf(name_forward_antimuon,"%s%d","plot_zboson_mass_forward_antimuon",iplot);
		plot_zboson_mass_forward_muon[iplot] = new TH1D(name_forward_muon, name_forward_muon,140,60,130);
		plot_zboson_mass_forward_antimuon[iplot] = new TH1D(name_forward_antimuon, name_forward_antimuon,140,60,130);
		plot_zboson_mass_forward_muon[iplot]->Sumw2();
		plot_zboson_mass_forward_antimuon[iplot]->Sumw2();
	}*/
}

void makeHists::saveHists()
{
	hf->cd();
	hf->Write();
	hf->Close();
}
 
