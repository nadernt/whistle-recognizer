#include "StdAfx.h"
#include ".\SndCardServices.h"

SndCard::SndCard(void)
{
}

SndCard::~SndCard(void)
{
}
//***********************************************************************************
/********************* Char Names ********************
"IN_DIGITAL"
"IN_HEADPHONES"
"IN_LINE"
"IN_MONITOR"
"IN_SPEAKERS"
"IN_TELEPHONE"
"IN_VOICE"
"IN_WAVE"
"OUT_ANALOG"
"OUT_AUXILIARY"
"OUT_COMPACTDISC"
"OUT_DIGITAL"
"OUT_LINE"
"OUT_MICROPHONE"
"OUT_SPEAKERS"
"OUT_SYNTHESIZER"
"OUT_TELEPHONE"
"OUT_WAVE"
*/
void SndCard::nSetVolume(char *SrcName,int ControlTypeID,DWORD VolSize) // ControlTypeID-> 0:Speaker 1:Input 2:OutPut
{
   DWORD nDeviceNameWord=nGetDevType(SrcName);
	
   // Open the mixer device
   HMIXER hmx;
   mixerOpen(&hmx, 0, 0, 0, 0);

   // Get the line info for the wave in destination line
	MIXERLINE mxl;
    mxl.cbStruct = sizeof(mxl);
	
	if(ControlTypeID==0)
		mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	else if(ControlTypeID==1)
			mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
	else 
		mxl.dwComponentType = nDeviceNameWord;		

    mixerGetLineInfo((HMIXEROBJ)hmx, &mxl,MIXER_GETLINEINFOF_COMPONENTTYPE);

   // Now find the microphone source line connected to this wave in
   // destination
   DWORD cConnections = mxl.cConnections;
   for(DWORD j=0; j<cConnections; j++){
      mxl.dwSource = j;
	  
	  if(ControlTypeID==0)
		mixerGetLineInfo((HMIXEROBJ)hmx, &mxl, MIXER_GETLINEINFOF_DESTINATION);
	  else
	    mixerGetLineInfo((HMIXEROBJ)hmx, &mxl, MIXER_GETLINEINFOF_SOURCE);

      if (ControlTypeID==2)
         break;

	  if (nDeviceNameWord == mxl.dwComponentType)
         break;
   }

   // Find a volume control, if any, of the microphone line
   LPMIXERCONTROL pmxctrl = (LPMIXERCONTROL)malloc(sizeof MIXERCONTROL);
   MIXERLINECONTROLS mxlctrl = {sizeof mxlctrl, mxl.dwLineID,
      MIXERCONTROL_CONTROLTYPE_VOLUME, 1, sizeof MIXERCONTROL, pmxctrl};
   if(!mixerGetLineControls((HMIXEROBJ) hmx, &mxlctrl, MIXER_GETLINECONTROLSF_ONEBYTYPE)){
      // Found!
      DWORD cChannels = mxl.cChannels;
      if (MIXERCONTROL_CONTROLF_UNIFORM & pmxctrl->fdwControl)
         cChannels = 1;

      LPMIXERCONTROLDETAILS_UNSIGNED pUnsigned = (LPMIXERCONTROLDETAILS_UNSIGNED)
         malloc(cChannels * sizeof MIXERCONTROLDETAILS_UNSIGNED);
      MIXERCONTROLDETAILS mxcd = {sizeof(mxcd), pmxctrl->dwControlID,
		cChannels, (HWND)0,
         sizeof MIXERCONTROLDETAILS_UNSIGNED, (LPVOID) pUnsigned};
      mixerGetControlDetails((HMIXEROBJ)hmx, &mxcd,MIXER_SETCONTROLDETAILSF_VALUE);
      // Set the volume to the middle  (for both channels as needed)
      pUnsigned[0].dwValue = pUnsigned[cChannels - 1].dwValue = VolSize;
//         (pmxctrl->Bounds.dwMinimum+pmxctrl->Bounds.dwMaximum)/2;
      mixerSetControlDetails((HMIXEROBJ)hmx, &mxcd,MIXER_SETCONTROLDETAILSF_VALUE);

    free(pmxctrl);
    free(pUnsigned);
   }
  else
    free(pmxctrl);
   mixerClose(hmx);
}

DWORD SndCard::nGetVolume(char *SrcName,int ControlTypeID) // ControlTypeID-> 0:Speaker 1:Input 2:OutPut
{
   DWORD nDeviceNameWord=nGetDevType(SrcName), I ;
	// Open the mixer device
   HMIXER hmx;
   mixerOpen(&hmx, 0, 0, 0, 0);
	
   // Get the line info for the wave in destination line
   MIXERLINE mxl;
    mxl.cbStruct = sizeof(mxl);
    if(ControlTypeID==0)
		mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	else if(ControlTypeID==1)
			mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
	else 
		mxl.dwComponentType = nDeviceNameWord;		

    mixerGetLineInfo((HMIXEROBJ)hmx, &mxl,MIXER_GETLINEINFOF_COMPONENTTYPE);

   // Now find the microphone source line connected to this wave in
   // destination
   DWORD cConnections = mxl.cConnections;
   for(DWORD j=0; j<cConnections; j++){
      mxl.dwSource = j;
	  
	  if(ControlTypeID==0)
		mixerGetLineInfo((HMIXEROBJ)hmx, &mxl, MIXER_GETLINEINFOF_DESTINATION);
	  else
	    mixerGetLineInfo((HMIXEROBJ)hmx, &mxl, MIXER_GETLINEINFOF_SOURCE);

      if (ControlTypeID==2)
         break;

	  if (nDeviceNameWord == mxl.dwComponentType)
         break;
   }
   // Find a volume control, if any, of the microphone line
   LPMIXERCONTROL pmxctrl = (LPMIXERCONTROL)malloc(sizeof MIXERCONTROL);
   MIXERLINECONTROLS mxlctrl = {sizeof mxlctrl, mxl.dwLineID,
      MIXERCONTROL_CONTROLTYPE_VOLUME, 1, sizeof MIXERCONTROL, pmxctrl};
   if(!mixerGetLineControls((HMIXEROBJ) hmx, &mxlctrl, MIXER_GETLINECONTROLSF_ONEBYTYPE)){
      // Found!
      DWORD cChannels = mxl.cChannels;
      if (MIXERCONTROL_CONTROLF_UNIFORM & pmxctrl->fdwControl)
         cChannels = 1;

      LPMIXERCONTROLDETAILS_UNSIGNED pUnsigned = (LPMIXERCONTROLDETAILS_UNSIGNED)
         malloc(cChannels * sizeof MIXERCONTROLDETAILS_UNSIGNED);
      MIXERCONTROLDETAILS mxcd = {sizeof(mxcd), pmxctrl->dwControlID,
		cChannels, (HWND)0,
         sizeof MIXERCONTROLDETAILS_UNSIGNED, (LPVOID) pUnsigned};
      mixerGetControlDetails((HMIXEROBJ)hmx, &mxcd,MIXER_GETCONTROLDETAILSF_VALUE);
		
		I=pUnsigned[0].dwValue; 

    free(pmxctrl);
    free(pUnsigned);
	return I; 
   }
  else
    free(pmxctrl);
   mixerClose(hmx);
	return 0;
}

//***********************************************************************************
/* In My sound card was :
"Wave Out Mix"
"Mono Out"
"Phone"
"Line In"
"Aux"
"Microphone"
"CD Player"
*/
void SndCard::nSourceSelector(char *SrcName)
{
 // Open the mixer device
   HMIXER hmx;
   mixerOpen(&hmx, 0, 0, 0, 0);

   // Get the line info for the wave in destination line
   MIXERLINE mxl;
    mxl.cbStruct      = sizeof(mxl);
    mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
    mixerGetLineInfo((HMIXEROBJ)hmx, &mxl,MIXER_GETLINEINFOF_COMPONENTTYPE);

   // Find a LIST control, if any, for the wave in line
   LPMIXERCONTROL pmxctrl = (LPMIXERCONTROL)malloc(mxl.cControls * sizeof MIXERCONTROL);
   MIXERLINECONTROLS mxlctrl = {sizeof mxlctrl, mxl.dwLineID, 0,mxl.cControls, 
								sizeof MIXERCONTROL, pmxctrl};
   mixerGetLineControls((HMIXEROBJ) hmx, &mxlctrl,MIXER_GETLINECONTROLSF_ALL);

   // Now walk through each control to find a type of LIST control. This
   // can be either Mux, Single-select, Mixer or Multiple-select.
   DWORD i;
   for(i=0; i < mxl.cControls; i++)
      if (MIXERCONTROL_CT_CLASS_LIST == (pmxctrl[i].dwControlType
&MIXERCONTROL_CT_CLASS_MASK))
         break;
   if (i < mxl.cControls) { // Found a LIST control
      // Check if the LIST control is a Mux or Single-select type
      BOOL bOneItemOnly = FALSE;
      switch (pmxctrl[i].dwControlType) {
         case MIXERCONTROL_CONTROLTYPE_MUX:
         case MIXERCONTROL_CONTROLTYPE_SINGLESELECT:
            bOneItemOnly = TRUE;
      }

      DWORD cChannels = mxl.cChannels, cMultipleItems = 0;
      if (MIXERCONTROL_CONTROLF_UNIFORM & pmxctrl[i].fdwControl)
         cChannels = 1;
      if (MIXERCONTROL_CONTROLF_MULTIPLE & pmxctrl[i].fdwControl)
         cMultipleItems = pmxctrl[i].cMultipleItems;

      // Get the text description of each item
      LPMIXERCONTROLDETAILS_LISTTEXT plisttext =
(LPMIXERCONTROLDETAILS_LISTTEXT)
         malloc(cChannels * cMultipleItems * sizeof
MIXERCONTROLDETAILS_LISTTEXT);
      MIXERCONTROLDETAILS mxcd = {sizeof(mxcd), pmxctrl[i].dwControlID,
cChannels,
         (HWND)cMultipleItems, sizeof MIXERCONTROLDETAILS_LISTTEXT,
(LPVOID) plisttext};
      mixerGetControlDetails((HMIXEROBJ)hmx, &mxcd,
MIXER_GETCONTROLDETAILSF_LISTTEXT);

      // Now get the value for each item
      LPMIXERCONTROLDETAILS_BOOLEAN plistBOOL =
(LPMIXERCONTROLDETAILS_BOOLEAN)
         malloc(cChannels * cMultipleItems * sizeof
MIXERCONTROLDETAILS_BOOLEAN);
      mxcd.cbDetails = sizeof MIXERCONTROLDETAILS_BOOLEAN;
      mxcd.paDetails = plistBOOL;
      mixerGetControlDetails((HMIXEROBJ)hmx, &mxcd,
MIXER_GETCONTROLDETAILSF_VALUE);

      // Select the "Microphone" item
      for (DWORD j=0; j<cMultipleItems; j = j + cChannels)
         if (0 == strcmp(plisttext[j].szName, SrcName))
            // Select it for both left and right channels
            plistBOOL[j].fValue = plistBOOL[j+ cChannels - 1].fValue = 1;
         else if (bOneItemOnly)
            // Mux or Single-select allows only one item to be selected
            // so clear other items as necessary
            plistBOOL[j].fValue = plistBOOL[j+ cChannels - 1].fValue = 0;
      // Now actually set the new values in
      mixerSetControlDetails((HMIXEROBJ)hmx, &mxcd,
MIXER_GETCONTROLDETAILSF_VALUE);

    free(pmxctrl);
    free(plisttext);
    free(plistBOOL);
   }
  else
    free(pmxctrl);
   mixerClose(hmx);
}

//***********************************************************************************
void SndCard::nMuteUnMute(char *SrcName,BOOL MuteState)
{

	DWORD nDeviceNameWord=nGetDevType(SrcName);
	// Open the mixer device
   HMIXER hmx;
    mixerOpen(&hmx, 0, 0, 0, 0);

   // Get the line info for the wave in destination line
   MIXERLINE mxl;
   BOOL OKi=FALSE;
   mxl.cbStruct = sizeof(mxl);
   if (MIXERLINE_COMPONENTTYPE_SRC_PCSPEAKER==nDeviceNameWord){
	    mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
		OKi=TRUE;}
   else
		mxl.dwComponentType = nDeviceNameWord;
   mixerGetLineInfo((HMIXEROBJ)hmx, &mxl, MIXER_GETLINEINFOF_COMPONENTTYPE);

   // Now find the microphone source line connected to this wave in
   // destination
    DWORD cConnections = mxl.cConnections;
   for(DWORD j=0; j<cConnections; j++){
      mxl.dwSource = j;
	  
	  if(OKi)
		mixerGetLineInfo((HMIXEROBJ)hmx, &mxl, MIXER_GETLINEINFOF_DESTINATION);
	  else
	    mixerGetLineInfo((HMIXEROBJ)hmx, &mxl, MIXER_GETLINEINFOF_SOURCE);

 	  if (nDeviceNameWord == mxl.dwComponentType)
         break;
   }


   // Find a mute control, if any
   LPMIXERCONTROL pmxctrl = (LPMIXERCONTROL)malloc(sizeof MIXERCONTROL);
   MIXERLINECONTROLS mxlctrl = {sizeof mxlctrl, mxl.dwLineID,MIXERCONTROL_CONTROLTYPE_MUTE, 
								1, sizeof MIXERCONTROL, pmxctrl};
   if(!mixerGetLineControls((HMIXEROBJ) hmx, &mxlctrl,MIXER_GETLINECONTROLSF_ONEBYTYPE)){
      // Found, so proceed
      DWORD cChannels = mxl.cChannels;
      if (MIXERCONTROL_CONTROLF_UNIFORM & pmxctrl->fdwControl)
         cChannels = 1;

      LPMIXERCONTROLDETAILS_BOOLEAN pBOOL =
         (LPMIXERCONTROLDETAILS_BOOLEAN) malloc(cChannels * sizeof MIXERCONTROLDETAILS_BOOLEAN);
      MIXERCONTROLDETAILS mxcd = {sizeof(mxcd), pmxctrl->dwControlID, cChannels, (HWND)0,
         sizeof MIXERCONTROLDETAILS_BOOLEAN, (LPVOID) pBOOL};
      mixerGetControlDetails((HMIXEROBJ)hmx, &mxcd,MIXER_SETCONTROLDETAILSF_VALUE);
      // Unmute the selected line (for both channels)
	  //CString T;
	  //T.Format("%d",pBOOL[cChannels - 1].fValue);
      //AfxMessageBox(T); 
      pBOOL[0].fValue = pBOOL[cChannels - 1].fValue = MuteState;
      mixerSetControlDetails((HMIXEROBJ)hmx, &mxcd,MIXER_SETCONTROLDETAILSF_VALUE);

    free(pmxctrl);
    free(pBOOL);
   }
  else
    free(pmxctrl);

   mixerClose(hmx);
}

//***********************************************************************************
//********************** 1 is Mute,0 is Not Mute,-1 is Unavailable*******************
int SndCard::nMuteGetState(char *SrcName)
{
	DWORD nDeviceNameWord=nGetDevType(SrcName);
	// Open the mixer device
	int RetVal;
   HMIXER hmx;
   mixerOpen(&hmx, 0, 0, 0, 0);

   // Get the line info for the wave in destination line
   MIXERLINE mxl;
   BOOL OKi=FALSE;
   mxl.cbStruct = sizeof(mxl);
   if (MIXERLINE_COMPONENTTYPE_SRC_PCSPEAKER==nDeviceNameWord){
	    mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
		OKi=TRUE;}
   else
		mxl.dwComponentType = nDeviceNameWord;
   mixerGetLineInfo((HMIXEROBJ)hmx, &mxl, MIXER_GETLINEINFOF_COMPONENTTYPE);

   // Now find the microphone source line connected to this wave in
   // destination
    DWORD cConnections = mxl.cConnections;
   for(DWORD j=0; j<cConnections; j++){
      mxl.dwSource = j;
	  
	  if(OKi)
		mixerGetLineInfo((HMIXEROBJ)hmx, &mxl, MIXER_GETLINEINFOF_DESTINATION);
	  else
	    mixerGetLineInfo((HMIXEROBJ)hmx, &mxl, MIXER_GETLINEINFOF_SOURCE);

 	  if (nDeviceNameWord == mxl.dwComponentType)
         break;
   }

   // Find a mute control, if any
   LPMIXERCONTROL pmxctrl = (LPMIXERCONTROL)malloc(sizeof MIXERCONTROL);
   MIXERLINECONTROLS mxlctrl = {sizeof mxlctrl, mxl.dwLineID,MIXERCONTROL_CONTROLTYPE_MUTE, 
								1, sizeof MIXERCONTROL, pmxctrl};
   if(!mixerGetLineControls((HMIXEROBJ) hmx, &mxlctrl,MIXER_GETLINECONTROLSF_ONEBYTYPE)){
      // Found, so proceed
      DWORD cChannels = mxl.cChannels;
      if (MIXERCONTROL_CONTROLF_UNIFORM & pmxctrl->fdwControl)
         cChannels = 1;

      LPMIXERCONTROLDETAILS_BOOLEAN pBOOL =
         (LPMIXERCONTROLDETAILS_BOOLEAN) malloc(cChannels * sizeof MIXERCONTROLDETAILS_BOOLEAN);
      MIXERCONTROLDETAILS mxcd = {sizeof(mxcd), pmxctrl->dwControlID, cChannels, (HWND)0,
         sizeof MIXERCONTROLDETAILS_BOOLEAN, (LPVOID) pBOOL};
      mixerGetControlDetails((HMIXEROBJ)hmx, &mxcd,MIXER_SETCONTROLDETAILSF_VALUE);
      if( pBOOL[cChannels - 1].fValue==1 || pBOOL[0].fValue==1)
			RetVal=1;
	  else
			RetVal= 0;
            

    free(pmxctrl);
    free(pBOOL);
   }
  else
  {
	RetVal=-1;
    free(pmxctrl);

  }
mixerClose(hmx);
return RetVal;
}

//***********************************************************************************
DWORD SndCard::nGetDevType(char *SrcName)
{
	if( stricmp(SrcName,"IN_DIGITAL") == 0 )
	{
		return  MIXERLINE_COMPONENTTYPE_DST_DIGITAL;
	}
	else if( stricmp(SrcName,"IN_HEADPHONES") == 0 )
	{
		return MIXERLINE_COMPONENTTYPE_DST_HEADPHONES;
	}
	else if( stricmp(SrcName,"IN_LINE") == 0 )
	{
		return MIXERLINE_COMPONENTTYPE_DST_LINE;
	}
	else if( stricmp(SrcName,"IN_MONITOR") == 0 )
	{
		return MIXERLINE_COMPONENTTYPE_DST_MONITOR;
	}
	else if( stricmp(SrcName,"IN_SPEAKERS") == 0 )
	{
		return MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	}
	else if( stricmp(SrcName,"IN_TELEPHONE") == 0 )
	{
		return MIXERLINE_COMPONENTTYPE_DST_TELEPHONE;
	}
	else if( stricmp(SrcName,"IN_VOICE") == 0 )
	{
		return MIXERLINE_COMPONENTTYPE_DST_VOICEIN;
	}
	else if( stricmp(SrcName,"IN_WAVE") == 0 )
	{
		return MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
	}
	else if( stricmp(SrcName,"OUT_ANALOG") == 0 )
	{
		return MIXERLINE_COMPONENTTYPE_SRC_ANALOG;
	}
	else if( stricmp(SrcName,"OUT_AUXILIARY") == 0 )
	{
		return MIXERLINE_COMPONENTTYPE_SRC_AUXILIARY;
	}
	else if( stricmp(SrcName,"OUT_COMPACTDISC") == 0 )
	{
		return MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC;
	}
	else if( stricmp(SrcName,"OUT_DIGITAL") == 0 )
	{
		return MIXERLINE_COMPONENTTYPE_SRC_DIGITAL;
	}
	else if( stricmp(SrcName,"OUT_LINE") == 0 )
	{
		return MIXERLINE_COMPONENTTYPE_SRC_LINE;
	}
	else if( stricmp(SrcName,"OUT_MICROPHONE") == 0 )
	{
		return MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE;
	}
	else if( stricmp(SrcName,"OUT_SPEAKERS") == 0 )
	{
		return MIXERLINE_COMPONENTTYPE_SRC_PCSPEAKER;
	}
	else if( stricmp(SrcName,"OUT_SYNTHESIZER") == 0 )
	{
		return MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER;
	}
	else if( stricmp(SrcName,"OUT_TELEPHONE") == 0 )
	{
		return MIXERLINE_COMPONENTTYPE_SRC_TELEPHONE;
	}
	else if( stricmp(SrcName,"OUT_WAVE") == 0 )
	{
		return MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;
	}
}