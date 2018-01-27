/* ########################################################################

   PICsimLab - PIC laboratory simulator

   ########################################################################

   Copyright (c) : 2010-2017  Luis Claudio Gambôa Lopes

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   For e-mail suggestions :  lcgamboa@yahoo.com
   ######################################################################## */

//main window

#include"picsimlab1.h"
#include"picsimlab1_d.cc"

CPWindow1 Window1;

//Implementation

#include"picsimlab2.h"
#include"picsimlab3.h"
#include"picsimlab4.h"
#include"picsimlab5.h"
#include"futils.h"


//extern char PROGDEVICE[100];
char SERIALDEVICE[100];

/*
//picstart plus
int prog_init(void);
int prog_loop(_pic * pic);
int prog_end(void);
*/


int crt;

void
CPWindow1::timer1_EvOnTime(CControl * control)
{
    if(!ondraw)
    {   
     if(!crt)   
       label1.SetColor(wxT("black"));
     crt=0;
     pboard->Draw(&draw1,scale);
    }
    else
    {
      label1.SetColor(wxT("red"));
      crt=1;
    }
    ondraw=1;
    tgo=1;//thread sync
    
};

void
CPWindow1::thread1_EvThreadRun(CControl*)
{
 //clock_t cstart=0, cend=0;
 //double cpu_time_used;

  //cstart = clock();

  while(!thread1.TestDestroy ())
  {
      
      if(tgo)
      {
        tgo=0;  
        pboard->Run_CPU();
        if(debug)pboard->DebugLoop();
        ondraw=0;
      }
      else
      {
#ifdef _WIN_
        Sleep(1);
#else
        usleep(1);
#endif        
      }

  }
  //cend = clock();
  //cpu_time_used = ((double) (cend - cstart)) / CLOCKS_PER_SEC;


   //printf("time=%f \n",cpu_time_used);
  
/*
   if(cpu_time_used > CPUTUSEMAX)
   {
      over+=(cpu_time_used-CPUTUSEMAX);
   
      //printf("over= %f   \n",over);

      if(over > 0.5)
      {

          over=0;

         int newf=((NSTEP-NSTEPKT)/NSTEPKT);//diminui clock
         
         if(newf <= 1)newf=1;    

         //printf("New Freq=%i\n",newf); 
 

         combo1.SetText(itoa(newf));//diminui clock
         combo1_EvOnComboChange(this);
 
       }  
   }
   else
   {
      over=0;
   }
*/  
  
//   cstart=0;
  // Application->ProcessEvents();

  
}

void
CPWindow1::timer2_EvOnTime(CControl * control)
{
   pboard->RefreshStatus(); 
   
   switch(cpustate)
   {
     case CPU_RUNNING:
       statusbar1.SetField(0,wxT("Running..."));break;
     case CPU_STEPPING:
       statusbar1.SetField(0,wxT("Stepping..."));break;
     case CPU_HALTED:  
       statusbar1.SetField(0,wxT("Halted!"));break;
     case CPU_BREAKPOINT:  
       statusbar1.SetField(0,wxT("BreakPoint!"));break;   
     case CPU_ERROR:  
       statusbar1.SetField(0,wxT("Error!"));break; 
   }
   
};



void
CPWindow1::draw1_EvMouseButtonPress(CControl * control, uint button, uint x, uint y,uint state)
{

  x=x/scale;
  y=y/scale;

  pboard->MouseButtonPress(button,x,y,state);
 
};


void
CPWindow1::draw1_EvMouseButtonRelease(CControl * control, uint button, uint x, uint y,uint state)
{
  
  x=x/scale;
  y=y/scale;
  
  pboard->MouseButtonRelease(button,x,y,state);

};


void
CPWindow1::draw1_EvKeyboardPress(CControl * control, uint key, uint x, uint y,uint mask)
{
  x=x/scale;
  y=y/scale;   
    
  pboard->KeyPress(key, x, y, mask); 

 
};


void
CPWindow1::draw1_EvKeyboardRelease(CControl * control, uint key, uint x, uint y,uint mask)
{
  x=x/scale;
  y=y/scale;  

  pboard->KeyRelease(key, x, y, mask);

};

void
CPWindow1::_EvOnCreate(CControl * control)
{
   char home[1024];
   String status;
 
if(!create)
{
  PATH=wxGetCwd(); 

  
#ifndef _WIN_
    share=String(_SHARE_);
#else
    share=wxGetCwd()+wxT("/")+String(_SHARE_);
#endif
    

#ifdef _WIN_
 Window3.combo1.SetItems(wxT("COM1,COM2,COM3,COM4,COM5,COM6,COM7,COM8,COM9,COM10,"));
 Window3.combo2.SetItems(wxT("COM1,COM2,COM3,COM4,COM5,COM6,COM7,COM8,COM9,COM10,"));
#else
 Window3.combo1.SetItems(wxT("/dev/ttyS0,/dev/ttyS1,/dev/ttyS2,/dev/ttyS3,/dev/tnt0,/dev/tnt1,/dev/tnt2,/dev/tnt3,/dev/tnt4,/dev/tnt5,"));
 Window3.combo2.SetItems(wxT("/dev/ttyS0,/dev/ttyS1,/dev/ttyS2,/dev/ttyS3,/dev/tnt0,/dev/tnt1,/dev/tnt2,/dev/tnt3,/dev/tnt4,/dev/tnt5,"));
#endif

}
create++;

//load options
  

  wxStandardPathsBase& stdp = wxStandardPaths::Get();
  strcpy(home,(char*)stdp.GetUserDataDir().char_str());
  
  Configure(control, home);
};

void
CPWindow1::Configure(CControl * control, const char * home)
{
  
  char line[256];
  char fname[1024];

  char *name;
  char *value;
 
  int i,j;
  int lc;
  float clk;
     
  String status;
    
  picrun=1;
  picpwr=1;
  picrst=0;
  
   sprintf(fname,"%s/picsimlab.ini",home);
  
   SERIALDEVICE[0]=0;
   //PROGDEVICE[0]=0;
   pboard=NULL;
   
  prefs.Clear(); 
  
  if(prefs.LoadFromFile(fname)) 
  {
    for(lc=0;lc< (int)prefs.GetLinesCount();lc++)      
    {
      strcpy(line,prefs.GetLine(lc).c_str());  
        
      name=strtok(line,"\t= ");
      strtok(NULL," ");
      value=strtok(NULL,"\"");
      if((name == NULL)||(value==NULL))continue;
#ifndef _WIN_
      if(!strcmp("lser",name))strcpy(SERIALDEVICE,value);
      //if(!strcmp("lprog",name))strcpy(PROGDEVICE,value);
#else
      if(!strcmp("wser",name))strcpy(SERIALDEVICE,value);
      //if(!strcmp("wprog",name))strcpy(PROGDEVICE,value);
#endif
      
      if(!strcmp(name,"lab"))
      {
        sscanf(value,"%i",&i);
        
        lab=i;
        lab_=i; 

        pboard = create_board(&lab,&lab_);         
      
        menu1_Microcontroller.DestroyChilds();
        String sdev = pboard->GetSupportedDevices();
        int f;
        int dc=0;
        while (sdev.size () > 0)
        {
          f = sdev.find (wxT(","));
          if(f < 0)break;
          MMicro[dc].SetFOwner(this);
          MMicro[dc].SetName("Micro_"+itoa(dc+1));
          MMicro[dc].SetText(sdev.substr (0, f));
          MMicro[dc].EvMenuActive=EVMENUACTIVE & CPWindow1::menu1_EvMicrocontroller;
          menu1_Microcontroller.CreateChild(&MMicro[dc]);
          sdev = sdev.substr (f + 1, sdev.size () - f - 1);
          dc++;
        }

 
        
      }
      if(!strcmp(name,"clock"))
      {
        sscanf(value,"%f",&clk);
        if(clk < 1)
        {
          combo1.SetText(String::Format("%2.1f",clk)); 
        }
        else
        {
          combo1.SetText(String::Format("%2.0f",clk));
        }
        combo1_EvOnComboChange(control);
      }
      
      if(!strcmp(name,"debug"))
      {
         sscanf(value,"%i",&debug);  
         togglebutton1.SetCheck(debug);
      }
      
      if(!strcmp(name,"osc_on"))
      {
         sscanf(value,"%i",&osc_on);  
      }
      
      if(!strcmp(name,"spare_on"))
      {
         sscanf(value,"%i",&spare_on);  
      }
      
      if(!strcmp(name,"spare_on"))
      {
         sscanf(value,"%i",&spare_on);  
      }
      
      if(!strcmp(name,"position"))
      {
         sscanf(value,"%i,%i",&i,&j);
         SetX(i);
         SetY(j);
      }
      
      
      if(!strcmp(name,"lpath"))
      {
         PATH=String(value,wxConvUTF8);
      }
      
      if(!strcmp(name,"lfile"))
      {
         FNAME=String(value,wxConvUTF8);
      }

      if(pboard != NULL)
        pboard->ReadPreferences(name,value); 

       Window4.ReadPreferences(name,value);
       Window5.ReadPreferences(name,value);
    }
  }
  else
  {
    printf("Error open config file \"%s\"!\n",fname);
    
    lab=1;//default  
    lab_=1;//default  
    
    pboard= new cboard_1();

#ifndef _WIN_   
   strcpy(SERIALDEVICE,"/dev/tnt2");
   //strcpy(PROGDEVICE,"/dev/tnt4");
#else
   strcpy(SERIALDEVICE,"com6");
   //strcpy(PROGDEVICE,"com8");
#endif
  }
  
  proc_=pboard->proc;
 
  SetTitle(wxT("PICSimLab - ")+ String(boards_list[lab-1])+wxT(" - ")+pboard->proc);
  
  

  filedialog1.SetDir(PATH);
 
  
  draw1.SetImgFileName(share+pboard->GetPictureFileName(),scale,scale);
  
  pboard->MSetSerial(SERIALDEVICE);
  
  sprintf(fname,"%s/mdump_%02i_%s.hex",home,lab,(const char *)pboard->proc.c_str ());

 
  switch(pboard->MInit(pboard->proc,fname,NSTEP*NSTEPKF))
  {
    case HEX_NFOUND:
      printf("File not found!\n"); 
      break;
    case HEX_CHKSUM:
      printf("File checksum error!\n"); 
      pboard->MEraseFlash();
      break;
  }

  pboard->Reset();
      

  /*
  if(prog_init() >= 0 ) 
     status=wxT("PStart:  Ok ");
  else
     status=wxT("PStart:Error");
  */
  status=wxT("");  

  if(debug)
  {
    if(pboard->DebugInit() == 0 ) 
      statusbar1.SetField(1,status+wxT("    Debug: On"));
    else
      statusbar1.SetField(1,status+wxT("    Debug: Error"));
  }
  else
  {
      statusbar1.SetField(1,status+wxT("    Debug: Off"));
  }

 
  statusbar1.SetField(0,wxT("Running..."));

  Application->ProcessEvents();

  thread1.Run();//parallel thread
  timer1.SetRunState(1);
  timer2.SetRunState(1);
  
  
  Window4.SetBaseTimer();
  
  sprintf(fname,"%s/parts_%02i.pcf",home,lab);
  Window5.LoadConfig (fname);
  
}

//Change  frequency
void
CPWindow1::combo1_EvOnComboChange(CControl * control)
{

  NSTEP= (int)(atof(combo1.GetText())*NSTEPKT); 
  
  NSTEPJ=NSTEP/JUMPSTEPS;

  pboard->MSetFreq(NSTEP*NSTEPKF);       
  Window4.SetBaseTimer();
  
  Application->ProcessEvents();
};

void
CPWindow1::saveprefs(String name, String value)
{
  char line[512];
  char *pname;  
  char *pvalue;
  
   for(int lc=0;lc< (int)prefs.GetLinesCount();lc++)      
    {
      strcpy(line,prefs.GetLine(lc).c_str());  
        
      pname=strtok(line,"\t= ");
      strtok(NULL," ");
      pvalue=strtok(NULL,"\"");
   
      if((pname == NULL)||(pvalue==NULL))continue;
      
      if(String(pname) == name)
      {
         prefs.SetLine(name+wxT("\t= \"")+value+wxT("\""),lc);
         
         //printf("refresh %s=%s\n",(const char *)name.c_str(),(const char *)value.c_str());
         return; 
      }
    }
    //printf("add     %s=%s\n",(const char *)name.c_str(),(const char *)value.c_str());
    prefs.AddLine(name+wxT("\t= \"")+value+wxT("\""));
}

void
CPWindow1::_EvOnDestroy(CControl * control)
{
  char home[1024];
  char fname[1024];
 
  
  Window4.Hide ();
  Window5.Hide ();
  
  timer1.SetRunState(0);
  timer2.SetRunState(0);
  thread1.Destroy ();
   
  

//write options
  wxTheApp->SetAppName(_T("picsimlab"));
  wxStandardPathsBase& stdp = wxStandardPaths::Get();
  strcpy(home,(char*)stdp.GetUserDataDir().char_str());
 
   CreateDir(home);

   sprintf(fname,"%s/picsimlab.ini",home);
  

  
    saveprefs(wxT("lab"),String::Format("%i",lab));
    saveprefs(wxT("clock"),combo1.GetText());
    saveprefs(wxT("debug"),itoa(debug));
    saveprefs(wxT("position"),itoa(GetX())+wxT(",")+itoa(GetY()));
    saveprefs(wxT("osc_on"),itoa(pboard->GetUseOscilloscope()));
    saveprefs(wxT("spare_on"),itoa(pboard->GetUseSpareParts()));
#ifndef _WIN_
    saveprefs(wxT("lser"),SERIALDEVICE);
    //saveprefs(wxT("lprog"),PROGDEVICE);
#else
    saveprefs("wser",SERIALDEVICE);
    //saveprefs("wprog",PROGDEVICE);
#endif
    saveprefs(wxT("lpath"),PATH);
    saveprefs(wxT("lfile"),FNAME);
        
    pboard->WritePreferences();
    
    Window4.WritePreferences ();
    Window5.WritePreferences ();
    
    prefs.SaveToFile(fname);
    
//write memory

  sprintf(fname,"%s/mdump_%02i_%s.hex",home,lab_,(const char*)proc_.c_str ());
  
  pboard->MDumpMemory(fname);
  
  
  pboard->MEnd();

  
  sprintf(fname,"%s/parts_%02i.pcf",home,lab_);
  Window5.SaveConfig (fname);
  Window5.DeleteParts();
  
  delete pboard;
  pboard =NULL;
  
  //refresh window position to window reopen in same position
  GetX();
  GetY();

};


void
CPWindow1::menu1_File_LoadHex_EvMenuActive(CControl * control)
{
  int pa;

        pa=picpwr; 
        picpwr=0;
        if(filedialog1.Run())
        {
  	  pboard->MEnd ();
          pboard->MSetSerial(SERIALDEVICE);
          
          switch(pboard->MInit(pboard->proc,filedialog1.GetFileName().char_str(),NSTEP*NSTEPKF))
          {
            case HEX_NFOUND:
              Message(wxT("File not found!")); 
              picrun=0;
              break;
            case HEX_CHKSUM:
              Message(wxT("File checksum error!")); 
              pboard->MEraseFlash ();
              picrun=0;
              break;
            case 0:
              picrun=1;
              break; 
          } 
 
          
          pboard->Reset();
            
 
          if(picrun) 
            SetTitle(wxT("PICSimLab - ")+ String(boards_list[lab-1])+wxT(" - ")+pboard->proc+wxT(" - ")+basename(filedialog1.GetFileName()));          
          else
            SetTitle(wxT("PICSimLab - ")+ String(boards_list[lab-1])+wxT(" - ")+pboard->proc);
  
          
        
          PATH=filedialog1.GetDir();
          FNAME=filedialog1.GetFileName();
        }
        picpwr=pa;
};

void
CPWindow1::menu1_File_Exit_EvMenuActive(CControl * control)
{
  WDestroy();
};

void
CPWindow1::menu1_Help_Contents_EvMenuActive(CControl * control)
{
  Window2.html1.SetLoadFile(share+wxT("doc/picsimlab.html"));   
  Window2.Show();
};


void
CPWindow1::menu1_Help_About_EvMenuActive(CControl * control)
{
  Message(wxT("Developed by L.C. Gamboa\n <lcgamboa@yahoo.com>\n Version: ")+String(wxT(_VERSION_)));
};

void
CPWindow1::menu1_Help_Examples_EvMenuActive(CControl * control)
{
  int pa;

        pa=picpwr; 
        picpwr=0;
        
        if(filedialog1.Run())
        {
          pboard->MEnd ();
          pboard->MSetSerial(SERIALDEVICE);
  	  switch(pboard->MInit (pboard->proc,filedialog1.GetFileName().char_str(),NSTEP*NSTEPKF))
          {
            case HEX_NFOUND:
              Message(wxT("File not found!")); 
              picrun=0;
              break;
            case HEX_CHKSUM:
              Message(wxT("File checksum error!")); 
              pboard->MEraseFlash ();
              picrun=0;
              break;
            case 0:
              picrun=1;
              break; 
          } 
          
          
          pboard->Reset();
  
 
          if(picrun) 
            SetTitle(wxT("PICSimLab - ")+ String(boards_list[lab-1])+wxT(" - ")+pboard->proc+wxT(" - ")+basename(filedialog1.GetFileName()));          
          else
            SetTitle(wxT("PICSimLab - ")+ String(boards_list[lab-1])+wxT(" - ")+pboard->proc);
  
          
        }
        filedialog1.SetDir(PATH);
        picpwr=pa;
};


//Resize
void
CPWindow1::_EvOnShow(CControl * control)
{
 double scalex,scaley;   

if(timer1.GetRunState())
{
  scalex=((Window1.GetWidth()-185)*1.0)/plWidth;
#ifdef _WIN_
  scaley=((Window1.GetHeight()-75)*1.0)/plHeight;
#else
  scaley=((Window1.GetHeight()-90)*1.0)/plHeight; 
#endif

  if(scalex < scaley)
    scale=scalex;
  else
    scale=scaley;


  draw1.SetWidth(plWidth*scale);
  draw1.SetHeight(plHeight*scale);
  

  draw1.SetImgFileName(share+pboard->GetPictureFileName(),scale,scale);
  pboard->OnShow();
  
  if(osc_on) 
  {
     menu1_Modules_Oscilloscope_EvMenuActive(this);
     osc_on=0;   
  }
  if(spare_on)
  { 
     menu1_Modules_Spareparts_EvMenuActive(this);
     spare_on=0;
  }
};

};


void
CPWindow1::menu1_File_Configure_EvMenuActive(CControl * control)
{
  Window3.ShowExclusive();
};


void
CPWindow1::menu1_File_ReloadLast_EvMenuActive(CControl * control)
{
  int pa;

        pa=picpwr; 
        picpwr=0;

     
          pboard->MEnd ();
          pboard->MSetSerial(SERIALDEVICE);
  	  
          switch(pboard->MInit (pboard->proc,FNAME.char_str(),NSTEP*NSTEPKF))
          {
            case HEX_NFOUND:
              Message(wxT("File not found!")); 
              picrun=0;
              break;
            case HEX_CHKSUM:
              Message(wxT("File checksum error!")); 
              pboard->MEraseFlash ();
              picrun=0;
              break;
            case 0:
              picrun=1;
              break; 
          } 
          
          pboard->Reset();
    
 
          if(picrun) 
            SetTitle(wxT("PICSimLab - ")+ String(boards_list[lab-1])+wxT(" - ")+pboard->proc+wxT(" - ")+basename(filedialog1.GetFileName()));          
          else
            SetTitle(wxT("PICSimLab - ")+ String(boards_list[lab-1])+wxT(" - ")+pboard->proc);
  
          
        
        
        picpwr=pa;
};

void 
CPWindow1::board_Event(CControl * control)
{
  pboard->board_Event(control);
};


void
CPWindow1::menu1_Modules_Oscilloscope_EvMenuActive(CControl * control)
{
  pboard->SetUseOscilloscope (1);
  Window4.Show ();
};


board * 
CPWindow1::GetBoard(void)
{
  return pboard;
}

void
CPWindow1::SetCpuState (unsigned char cs) 
{
  cpustate=cs;
}


void
CPWindow1::menu1_Modules_Spareparts_EvMenuActive(CControl * control)
{
  
 
  pboard->SetUseSpareParts (1);
  Window5.Show ();
  Window5.timer1.SetRunState (1);
  

};


void
CPWindow1::menu1_Tools_SerialTerm_EvMenuActive(CControl * control)
{
#ifdef _WIN_  
  wxExecute(share+wxT("/../tools/cutecom/cutecom.exe"));
#else
  wxExecute("cutecom");
#endif  
};

//Change board
void
CPWindow1::menu1_EvBoard(CControl * control)
{
  lab_= lab;
  lab= (int)(atof(((CItemMenu*)control)->GetText ()));

  _EvOnDestroy(control);
  _EvOnCreate(control);
  _EvOnShow(control);
};

//change microcontroller
void
CPWindow1::menu1_EvMicrocontroller(CControl * control)
{
   proc_=pboard->proc;
   pboard->proc=((CItemMenu*)control)->GetText ();
   
   SetTitle(wxT("PICSimLab - ")+ String(boards_list[lab-1])+wxT(" - ")+pboard->proc);
   
  _EvOnDestroy(control);
  _EvOnCreate(control);
  _EvOnShow(control);
};



void
CPWindow1::togglebutton1_EvOnToggleButton(CControl * control)
{
   int osc_on= pboard->GetUseOscilloscope();
   int spare_on= pboard->GetUseSpareParts();
   
   debug=togglebutton1.GetCheck();
           
  _EvOnDestroy(control);
  _EvOnCreate(control);
  _EvOnShow(control);
  
  if(osc_on) menu1_Modules_Oscilloscope_EvMenuActive(this);
  if(spare_on) menu1_Modules_Spareparts_EvMenuActive(this);
          

};


void
CPWindow1::menu1_File_SaveWorkspace_EvMenuActive(CControl * control)
{
  filedialog2.SetType(wxFD_SAVE|wxFD_CHANGE_DIR);  
  if(filedialog2.Run())
  {
     char home[1024];
     char fname[1024];
     
     if(wxFileExists(filedialog2.GetFileName()))
     {
       
         if(!Dialog("Overwriting file: "+basename(filedialog2.GetFileName())+"?"))
             return;
     }
     
     //write options
     wxTheApp->SetAppName(_T("picsimlab"));
     wxStandardPathsBase& stdp = wxStandardPaths::Get();
     strcpy(home,(char*)stdp.GetTempDir().char_str());
     strcat(home,"/picsimlab_workspace/");
 
     RemoveDir(home);
     
     CreateDir(home);     

     sprintf(fname,"%s/picsimlab.ini",home);
  

     saveprefs(wxT("lab"),String::Format("%i",lab));
     saveprefs(wxT("clock"),combo1.GetText());
     saveprefs(wxT("debug"),itoa(debug));
#ifndef _WIN_
     saveprefs(wxT("lser"),SERIALDEVICE);
#else
     saveprefs("wser",SERIALDEVICE);
#endif
     saveprefs(wxT("lpath"),PATH);
     saveprefs(wxT("lfile"),FNAME);
        
     pboard->WritePreferences();
    
     Window4.WritePreferences ();
    
     prefs.SaveToFile(fname);
    
//write memory
     sprintf(fname,"%s/mdump_%02i_%s.hex",home,lab_,(const char*)proc_.c_str ());
  
     pboard->MDumpMemory(fname);
     
     sprintf(fname,"%s/parts_%02i.pcf",home,lab_);
     Window5.SaveConfig (fname);
     
     ZipDir(home, filedialog2.GetFileName());
     
     RemoveDir(home);
  }
};

void
CPWindow1::menu1_File_LoadWorkspace_EvMenuActive(CControl * control)
{
  filedialog2.SetType(wxFD_OPEN|wxFD_CHANGE_DIR);
  if(filedialog2.Run())
  {
      char home[1024];
      char fzip[1024];
      
      //write options
      wxTheApp->SetAppName(_T("picsimlab"));
      wxStandardPathsBase& stdp = wxStandardPaths::Get();
      strcpy(fzip,(char*)stdp.GetTempDir().char_str());
      strcat(fzip,"/");
      
      strcpy(home,(char*)stdp.GetTempDir().char_str());
      strcat(home,"/picsimlab_workspace/");
 
      RemoveDir(home);
 
      UnzipDir(filedialog2.GetFileName(),fzip);
      
     _EvOnDestroy(control);
     Configure(control,home);
     _EvOnShow(control);  
     
     RemoveDir(home);
  }
};


