//
//  main.cpp
//  aneeshsimul
//
//  Created by Aneesh Zutshi on 12/10/11.
//  Copyright (c) 2011 FCT/UNL. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

#define MAXAGENTS 20000
#define directinfluencefactor 1
#define nbinfluencefactor 1
#define globalinfluencefactor 1
#define influencerate 1             //determines rate of change of influence
#define maxcompetitors 10
#define maxnumofplans 3
#define totalnumberofservices 10    //additional services offered


class position
{   public:
    int x;
    int y;
    position pos(int sel)
    {
        position ret;
        switch (sel)
        {
            case 1:  ret.x=x; ret.y=y+1; break;
            case 2:  ret.x=x; ret.y=y-1; break;
            case 3:  ret.x=x+1; ret.y=y; break;
            case 4:  ret.x=x-1; ret.y=y; break;
            case 5:  ret.x=x+1; ret.y=y+1; break;
            case 6:  ret.x=x-1; ret.y=y-1; break;
            case 7:  ret.x=x+1; ret.y=y-1; break;
            case 8:  ret.x=x-1; ret.y=y+1; break;
        }
        return ret;
    }
};

struct influence
{
    float web=0;    //percent
    float tel=0;    //percent
    float sales=0;  //percent
};

struct additionalservices
{
    std::string description;
    std::string unit;    
    float priceperunit;
};



class businessmodel
{
public:
    
    float webadvertising;       //advertising effort
    float television;           
    float sales;                //sales force presence
    float price[maxnumofplans]; //cost benchmark for each tariff plan. In case of freemium plan price[0] is always 0.
    // price[0]=0;
    additionalservices service[totalnumberofservices];
    
};

class agent
{
    position location;
    influence infl;
    float totalinfluence=0;
    float directinfluence;
    float avgnbinfluence;
    int clientof=0;                   //variable to store agent is client of which company number 0 if not a client of none 
    float satisfaction;             //We are currently assuming that satisfaction is a single variable between 0 and 1. 
    
    
public:
    void step(float globalinfl)
    {   
        float newinfl;
        directinfluence=(infl.web + infl.tel + infl.sales)/3;
        newinfl=((globalinfluencefactor*globalinfl)+(nbinfluencefactor*avgnbinfluence)+(directinfluencefactor*directinfluence))/(globalinfluencefactor+nbinfluencefactor+directinfluencefactor);
        totalinfluence=(totalinfluence+(influencerate*newinfl))/(1+influencerate);
    }
        
    void locate(int a, int b)
    {
        location.x=a;
        location.y=b;
    }
    
    float getdirectinfluence()
    {
        return totalinfluence;
    }
    
    position returnloc()
    {
        return location;
    }
    
    void updatenbinfluence(float upd)
    {
        avgnbinfluence=upd;
    }
    
};

class global
{
    agent client[MAXAGENTS];
    businessmodel company1;
    int NUMCLIENTS;
    float totalglobalinfluence;
    float avgglobalinfluence;
    
public:
    global(int numclients)
    {
        NUMCLIENTS=numclients;
        int l=0;
        for (int i=0; l<NUMCLIENTS; i++)
        {
            for (int j=0; j<100; j++)
            {
                client[l].locate(i,j);
                l++;
            }
        }
        
        
/*********************** initialisation of business model parameter **************************/        
        {
            company1.service[0].description="timestamps";
            company1.service[0].unit="Euro/Purchase";
            company1.service[0].priceperunit=0.11;
            company1.webadvertising=0.88;
            company1.television=0.34;
            company1.sales=0.23;
            company1.price[0]=34;
            company1.price[1]=45;
            company1.price[2]=78;
        }
/*********************************************************************************************/        
        
    }
    
    void step()
    {
        totalglobalinfluence=0;
        int l=0;
        for (int i=0; l<NUMCLIENTS; i++)
        {
            for (int j=0; j<100; j++)
            {
                totalglobalinfluence+=client[l].getdirectinfluence();
                l++;
            }
        }
        avgglobalinfluence=totalglobalinfluence/NUMCLIENTS;
        
        l=0;
        for (int i=0; l<NUMCLIENTS; i++)
        {
            for (int j=0; j<100; j++)
            {
                client[l].updatenbinfluence(avgnbinfluence(l));
                l++;
            }
        }
        
        l=0;
        for (int i=0; l<NUMCLIENTS; i++)
        {
            for (int j=0; j<100; j++)
            {
                client[l].step(avgglobalinfluence);
                l++;
            }
        }
    }
    
    float avgnbinfluence(int num)
    {
        int refnum;
        float suminfluence=0;
        position posi=client[num].returnloc();
        for (int i=1; i<8; i++)
        {
            refnum=numfrompos(posi.pos(i));
            suminfluence+=client[refnum].getdirectinfluence();
        }
        return (suminfluence/8);
    };
    
    int numfrompos(position posref)
    {
        int l=0;
        int posfin=0;
        int found=0;
        position poscheck;
        for (int i=0; l<NUMCLIENTS; i++)
        {
            for (int j=0; j<100; j++)
            {
                poscheck=client[l].returnloc();
                if(poscheck.x==i && poscheck.y==j) {posfin=l; found=1;}
                l++;
            }
        }
        return posfin;
    }

};




int main ()
{    
    global simulation(1000);
    for (int i=0; i<10000; i++)
        {
            simulation.step();
            cout<<i<<"\n";
            sleep(1000);
        }
    return 0;
}

