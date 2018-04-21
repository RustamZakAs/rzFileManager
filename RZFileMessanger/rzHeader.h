#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define CRT_SECURE_NO_WARNINGS
//#define UNICODE
//#define _UNICODE
#include <iostream>
#include <windows.h>
#include <fstream>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstring>
#include <string.h>
#include <string>
#include <iomanip>
#include <Wincon.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctime>
#include <string.h>
#include <filesystem>
#include <direct.h>

#define RZ "Rustam Zak. As."
#define default_color 07
#define allff "*.*"
#define delimetrff "\\"
#define showStrLen 50

using namespace std;

class ParamCC {
private:

public:
	ParamCC() { SetScreen(); };
	const void SetCoord(const short x, const short y); //координаты
	const void SetColor(const short color); // цвет
	const void SetScreen(); //экран
protected:

};

class FolderFileList : public ParamCC {
private:
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	struct fList {
		char *fPath = nullptr;
		char *fName = nullptr;
	};
	short l_color = 79;
	int l_index = 0;
	int l_count = 0;

	char *xfPath = nullptr;
	char *xfPathOld = nullptr;
	char *xfNameOld = nullptr;
	bool MoveOrC = false;

	fList *rzfm_fList = nullptr;
	char **LogicDriveList = nullptr;
	char *strConversion = nullptr;
public:
	FolderFileList();
	const void rzVirtLoadWindows(const short coordX = 3, const short coordY = 7, const short sleepInt = 1, const short colorInt = 255);
	const void rzVirtLoad(const short coordX = 3, const short coordY = 7, const short sleepInt = 1, const short colorInt = 255);
	const void rzIntro();
	const void rzGetLogicalDisk();
	//**********************************************************************************************************************************************************
	
	//**********************************************************************************************************************************************************
	const void rzSetListIndex(const int l_index) {
		this->l_index = l_index;
	}
	//**********************************************************************************************************************************************************
	const void rzIncListIndex() {
		(this->l_index)++;
		if (this->l_index >= this->l_count) this->l_index = this->l_count - 1;
	}
	//**********************************************************************************************************************************************************
	const void rzDecListIndex() {
		(this->l_index)--;
		if (this->l_index < 0) this->l_index = 0;
	}
	//**********************************************************************************************************************************************************
	const void rzMaxListIndex() {
		this->l_index = l_count - 1;
	}
	//**********************************************************************************************************************************************************
	const void rzMinListIndex() {
		this->l_index = 0;
	}
	//**********************************************************************************************************************************************************
	const int rzGetCount() {
		return this->l_count;
	}
	//**********************************************************************************************************************************************************
	const int rzGetListIndex() {
		return this->l_index;
	}
	//**********************************************************************************************************************************************************
	const char* rzGetListPath(const int xcount) {
		return this->rzfm_fList[xcount].fPath;
	}
	//**********************************************************************************************************************************************************
	const char* rzGetPath() {
		return this->xfPath;
	}
	//**********************************************************************************************************************************************************
	const char *rzGetListName(const int xcount) {
		return this->rzfm_fList[xcount].fName;
	}
	//**********************************************************************************************************************************************************
	const void rzSetListPath(const char *xfPath) {
		if (this->xfPath) delete[] this->xfPath;
		this->xfPath = new char[strlen(xfPath) + 1]{};
		strcpy(this->xfPath, xfPath);
	}
	//**********************************************************************************************************************************************************
	const void rzFolderInfo();
	char* const rzGetFullPathToAllFiles();
	const void rzFindFoldersFilesCount();
	const void rzCreateFileList();
	const void rzPrintList();
	const void rzPrintMenyu(const int color_menyu = 150);
	const void rzFileCopy(const char *old_filename_path, const char *new_filename_path);
	const void rzPrintForm(const short X, const short Y,
						   const short zcolor, const short zcolorhead,
						   const short zwidth, const short zheigth,
						   const short zleftindent, const short ztopindent,
						   const char  *zheadtext);
	const short rzOtherMenyu(const short X = 15, const short Y = 5);
	char* const rzInsertFileName();
	char* const rzEnterNumber(const short charlen = 13, bool telnumber = false);
	char* const rzEnterText(const short charlen = 255);
	const void rzPassEnter();
	const void rzPassBack();
	const short rzGetType(const char *fullPath, bool visible = false);
	char* const rzStringConversion(const char *filename, const short length);
	const void rzShowTxtFile(const char *filename);
	const bool rzEqualityText(const char *filename1, const char *filename2);
	~FolderFileList() {
		if (rzfm_fList) {
			delete[] rzfm_fList;
		}
	};
}/* rzfm*/;