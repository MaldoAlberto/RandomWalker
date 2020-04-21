#include <iostream>
#include <time.h>
#include <math.h>
#include <fstream>
#include "TH1.h"
#include "TH1D.h"
#include <string.h>
#include <TRandom.h>
#include <TMath.h>
#include <algorithm>


int posicionfinal()
{
  float random;
  int m = 0,i = 0,j = 0;
  int n1=0,n2=0,N=10,exp=1000000;
  Double_t x1=0,x2=2;
  time_t t;
  int peso = 1;  
  TString titulo = "Pesos ";
  TCanvas *c1;



  int tam = 1000000;

	float arreglo[tam],arregloN[tam];
	string line;
	int sizet = 0; 
	

	ifstream myfile ("salidaY_normalizado.txt");
	  if (myfile.is_open())
	  {
	    while ( getline (myfile,line) )
	    {
		if(sizet < tam)
		      	arreglo[sizet] = stof(line);
		//cout << arreglo[sizet] << endl;
		sizet++;
	    }
	    myfile.close();
	  }
	  
          int locmin = TMath::LocMin(tam,arreglo);
	  int locmax = TMath::LocMax(tam,arreglo);
	  int ind[tam];	
			  


         // cout << locmin << " , " << locmax << endl;
	//  cout << arreglo[locmin] << " , " << arreglo[locmax] << endl;

	
	  for (int i = 0; i < tam; i++)
	{
		arregloN[i] = (arreglo[i]-arreglo[locmin])/(760);
		cout << arregloN[i] << endl; 
	}
	//cout << arregloN[locmin] << " , " << arregloN[locmax] << endl;

	


  while (peso < 11){
  titulo.Form("Pesos %d - %d",peso, 10-peso); 
  cout << titulo ;
  auto c1 = new TCanvas(titulo, "Caminata Aleatoria",900,600);
  auto h1 = new TH1F("h1", titulo, 24 , 0, 2);
  auto h3 = new TH1F("h3", "Posicion final",13 , -1, 11);
  auto h4 = new TH1F("h4", "Posicion final",13, -11, 1);
  auto h5 = new TH1F("h5", "Posicion final",21, -10, 10);
  THStack * hs2 = new THStack("hs2","Pasos a la izquierda y derecha");
  gStyle->SetOptStat("ne");
  
 
  srand((unsigned) time(&t));
  for(j = 0;j < exp; j++)
    {
      for(i = 0;i < N; i++)
	{
	//  random = rand()%10;
	  if (arregloN[i+j] < .1*float(peso)){
	    n1 = n1 + 1;
	  }
	  if(arregloN[i+j] >= .1*float(peso)){
	    n2 = n2 + 1;
	  }
	  m=n1-n2;
	}

      h1->Fill("n1",n1);

      h1->Fill("n2",n2);
      n1=0;
      n2=0;
      m=0;
    }

  for(j = 0;j < exp; j++)
    {
      for(i = 0;i < N; i++)
	{
	  //random = rand()%10;
	  if (arregloN[i+j] >= .1*float(peso)){
	    n1 = n1 + 1;
	  }
	  if(arregloN[i+j] < .1*float(peso)){

	    n2 = n2 - 1;
	  }
	  m=n1+n2;
	  
	  //cout << "m " << m << " n1 " << n1 << " n2 " << n2 << endl;		
	}
      
      h5->Fill(m);
      h3->Fill(n1);
      h4->Fill(n2);
      
      n1=0;
      n2=0;
      m=0;
    }

  c1->Divide(3,1);
 
  h1->SetFillColor(kAzure+9);
  h1->SetBarWidth(0.85);


  h3->SetFillColor(kAzure+9);
  h3->SetBarWidth(0.85);
  h4->SetBarWidth(0.85);
  h4->SetFillColor(kGreen+9);


  Double_t scale = 1/h1->Integral();
  h1->Scale(scale);


  scale = 1/h3->Integral();
  h3->Scale(scale);
  scale = 1/h4->Integral();
  h4->Scale(scale);
  c1->cd(1);
  h1->GetXaxis()->SetRange(1,3);
  h1->Draw("HISTBAR");

  c1->Modified();
  c1->Update();

  auto legend = new TLegend(0.3,0.9,0.68,0.8);//der,arriba,,izq
  legend->SetTextSize(0.03);
  legend->AddEntry(h1,"h1:Pasos a la izquierda","f");
  legend->Draw();


  Double_t meanh1,stdh1;
  meanh1 = h1->GetMean();
  stdh1 = h1->GetRMS();
  TPaveText *pt = new TPaveText(0.71,0.8,0.98,0.98,"brNDC");
  pt->SetFillColor(18);
  pt->SetTextAlign(12);
  pt->AddText("Stats");
  pt->AddText(Form("Entries = %g",h1->GetEntries()*5));
  pt->AddText("");

  pt->Draw();

  c1->cd(2);
  h5->SetBarWidth(0.85);
  h5->SetFillColor(30);
  scale = 1/h5->Integral();
  h5->Scale(scale);
  h5->Draw("HISTBAR");
  h5->GetYaxis()->SetTitle("1/N");
  h5->GetXaxis()->SetTitle("m");


  auto legend2 = new TLegend(0.3,0.9,0.68,0.8);//der,arriba,,izq
  legend2->SetTextSize(0.03);
  legend2->AddEntry(h5,"Pasos finales","f");
  legend2->Draw();

  Double_t meanh5,stdh5;
  meanh5 = h5->GetMean();
  stdh5 = h5->GetRMS();
  TPaveText *pt2 = new TPaveText(0.74,0.8,0.98,0.98,"brNDC");
  pt2->SetFillColor(18);
  pt2->SetTextAlign(12);
  pt2->AddText("Stats");
  pt2->AddText(Form("Entries = %g",h5->GetEntries()));
  pt2->AddText(Form("RMS  = %g",stdh5));
  pt2->AddText(Form("Mean = %g",meanh5));

  pt2->Draw();


  c1->cd(3);
  //c1->Size(100,20);
  hs2->Add(h3);
  hs2->Add(h4);
  hs2->Draw("HISTBAR");


  hs2->GetYaxis()->SetTitle("1/N");
  hs2->GetXaxis()->SetTitle("n1,n2");
  c1->Modified();
  c1->Update();

  auto legend3 = new TLegend(0.3,0.9,0.68,0.8);//der,arriba,,izq
  legend3->SetTextSize(0.03);
  legend3->AddEntry(h3,"h3:Pasos a la derecha","f");
  legend3->AddEntry(h4,"h4:Pasos a la izquierda","f");
  legend3->Draw();

   Double_t meanh3,meanh4,stdh3,stdh4;
  meanh3 = h3->GetMean();
  meanh4 = h4->GetMean();
  stdh3 = h3->GetRMS();
  stdh4 = h4->GetRMS();
  TPaveText *pt3 = new TPaveText(0.81,0.8,0.98,0.98,"brNDC");
  pt3->SetFillColor(18);
  pt3->SetTextAlign(12);
  pt3->AddText("Stats");
  pt3->AddText(Form("Mean h3 = %g",meanh3));
  pt3->AddText(Form("RMS h3 = %g",stdh3));
  pt3->AddText("");
  pt3->AddText(Form("Mean h4 = %g",meanh4));
  pt3->AddText(Form("RMS h4 = %g",stdh4));
  pt3->Draw();


  c1->Print(titulo,".png");
  
  peso = peso + 1;
  }
  return 0;
}







