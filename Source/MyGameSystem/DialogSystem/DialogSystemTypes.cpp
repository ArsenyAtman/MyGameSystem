// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogSystemTypes.h"
#include "Sound/DialogueWave.h"
#include "Sound/DialogueVoice.h"

bool FDialogCueStruct::operator == (const FDialogCueStruct& CueInfo)//1, const FDialogCueStruct& CueInfo2)
{
	return this->DialogueVoice == CueInfo.DialogueVoice && this->DialogueWave == CueInfo.DialogueWave && this->DialogCueType == CueInfo.DialogCueType;
}

bool FDialogCueStruct::operator != (const FDialogCueStruct& CueInfo)//1, const FDialogCueStruct& CueInfo2)
{
	return !(*this == CueInfo);
}

bool FDialogSelectionStruct::operator == (const FDialogSelectionStruct& SelectionInfo)//, const FDialogSelectionStruct& SelectionInfo2)
{
	if (this->bWithTimer == SelectionInfo.bWithTimer && this->Time == SelectionInfo.Time && this->Options.Num() == SelectionInfo.Options.Num())
	{
		for (int i = this->Options.Num() - 1; i >= 0; --i)
		{
			if (this->Options[i] != SelectionInfo.Options[i])
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

bool FDialogSelectionStruct::operator != (const FDialogSelectionStruct& SelectionInfo)//, const FDialogSelectionStruct& SelectionInfo2)
{
	return !(*this == SelectionInfo);
}