#include "rzHeader.h"

const void ParamCC::SetCoord(const short x, const short y) {
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(hcon, coord);
}

const void ParamCC::SetColor(const short color) {    //Цвет
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, color);
}

const void ParamCC::SetScreen() {
	HANDLE hCon = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	if (hCon != INVALID_HANDLE_VALUE) {
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
		cfi.nFont = 07;
		cfi.dwFontSize.X = 10;
		cfi.dwFontSize.Y = 8;
		cfi.FontFamily = FF_DECORATIVE;
		cfi.FontWeight = 400;
		wcscpy(cfi.FaceName, L"Arial Cyr");
		SetCurrentConsoleFontEx(hCon, FALSE, &cfi);
	}
}

FolderFileList::FolderFileList() {
	setlocale(LC_ALL, "Russian");
	system("color 07");
	rzIntro();
	rzGetLogicalDisk();
	rzFindFoldersFilesCount();
	rzfm_fList = new fList[rzGetCount() + 1]{};
	rzCreateFileList();
	system("cls");
	rzPrintMenyu();
	rzPrintList();
	cout << endl;
	rzPrintMenyu();
	rzFolderInfo();
}

char* const FolderFileList::rzGetFullPathToAllFiles() {
	char *FullPathToAllFiles = new char[strlen(rzGetPath())
		+ strlen(delimetrff) + strlen(allff) + 1]{};
	strcpy(FullPathToAllFiles, rzGetPath());
	strcat(FullPathToAllFiles, delimetrff);
	strcat(FullPathToAllFiles, allff);
	return FullPathToAllFiles;
}

const bool FolderFileList::rzEqualityText(const char *filename1, const char *filename2) {
	if (strlen(filename1) == strlen(filename2)) {
		for (short i = 0; i < (short)strlen(filename1); i++) {
			if (short(filename1[i]) != short(filename2[i])) return 0; else continue;
		}
	} else return 0;
	return 1;
}

const void FolderFileList::rzPrintMenyu(const int color_menyu) {

	SetColor(default_color);
	cout << " 1.";
	SetColor(color_menyu);
	cout << "F1 Help";

	SetColor(default_color);
	cout << " 2.";
	SetColor(color_menyu);
	cout << "F2 Rename";

	SetColor(default_color);
	cout << " 3.";
	SetColor(color_menyu);
	cout << "F3 Home";

	SetColor(default_color);
	cout << " 4.";
	SetColor(color_menyu);
	cout << "F4 Exit";

	SetColor(default_color);
	cout << " 5.";
	SetColor(color_menyu);
	cout << "F5 Refresh";

	SetColor(default_color);
	cout << " 6.";
	SetColor(color_menyu);
	cout << "F6 File info";
	SetColor(default_color);
}

const short FolderFileList::rzOtherMenyu(const short X, const short Y) {
	short xwidth = 20, xheigth = 11, xcolor = 144, xindent = 2, xMenyuCount = 8;

	rzPrintForm(X, Y, xcolor, 81, xwidth, xheigth, xindent, 1, "FILE MENYU");

	string *xMenyu = new string[xMenyuCount]{};
	xMenyu[0] = "      COPY      ";
	xMenyu[1] = "      MOVE      ";
	xMenyu[2] = "      PASTE     ";
	xMenyu[3] = "     DELETE     ";
	xMenyu[4] = "     RENAME     ";
	xMenyu[5] = "   PROPERTIES   ";
	xMenyu[6] = "   CREATE FILE  ";
	xMenyu[7] = "CREATE DIRECTORY";

	short xkey = 0;
	short m_index = 0;

	for (short i = 0; i < xMenyuCount; i++) {
		SetCoord(X + 1 + xindent, Y + 3 + i);
		if (m_index == i) {
			SetColor(71);
			cout << xMenyu[i];
			SetColor(xcolor);
		}
		else {
			SetColor(xcolor);
			cout << xMenyu[i];
		}
	}

	while (true)
	{
		do {
			xkey = _getch();
		} while (xkey == 224);

		cout << xkey;

		if (xkey == 80) {
			m_index++;
			m_index > xMenyuCount - 1 ? m_index = xMenyuCount - 1 : m_index;
		}
		if (xkey == 72) {
			m_index--;
			m_index < 0 ? m_index = 0 : m_index;
		}
		if (xkey == 13) break;
		for (short i = 0; i < xMenyuCount; i++) {
			SetCoord(X + 1 + xindent, Y + 3 + i);
			if (m_index == i) {
				SetColor(71);
				cout << xMenyu[i];
				SetColor(xcolor);
			}
			else {
				SetColor(xcolor);
				cout << xMenyu[i];
			}
		}
		if (xkey == 27) {
			SetColor(default_color);
			system("cls");
			rzPrintList();
			rzPrintMenyu();
			return 0;
		}
	}

	if (m_index == 0) {  //copy
		if (this->xfPathOld) delete[] this->xfPathOld;
		this->xfPathOld = new char[strlen(rzGetPath()) + strlen(delimetrff) + strlen(rzfm_fList[rzGetListIndex()].fName) + 1] {};
		strcpy(this->xfPathOld, rzGetPath());
		strcat(this->xfPathOld, delimetrff);
		strcat(this->xfPathOld, rzfm_fList[rzGetListIndex()].fName);
		this->MoveOrC = false;

		if (this->xfNameOld) delete[] this->xfNameOld;
		this->xfNameOld = new char[strlen(rzfm_fList[rzGetListIndex()].fName)] {};
		strcpy(this->xfNameOld, rzfm_fList[rzGetListIndex()].fName);
	}
	else if (m_index == 1) { //move
		if (this->xfPathOld) delete[] this->xfPathOld;
		this->xfPathOld = new char[strlen(rzGetPath()) + strlen(delimetrff) + strlen(rzfm_fList[rzGetListIndex()].fName) + 1]{};
		strcpy(this->xfPathOld, rzGetPath());
		strcat(this->xfPathOld, delimetrff);
		strcat(this->xfPathOld, rzfm_fList[rzGetListIndex()].fName);
		this->MoveOrC = true;

		if (this->xfNameOld) delete[] this->xfNameOld;
	
		this->xfNameOld = new char[strlen(rzfm_fList[rzGetListIndex()].fName)]{};
		strcpy(this->xfNameOld, rzfm_fList[rzGetListIndex()].fName);
	}
	else if (m_index == 2) { //paste
		if (this->MoveOrC) {  // if move
			char *xxx = new char[strlen(rzGetPath()) 
				+ strlen(delimetrff) 
				+ strlen(this->xfNameOld) + 1] {};
			strcpy(xxx, rzGetPath());
			strcat(xxx, delimetrff);
			strcat(xxx, this->xfNameOld);
			int fType = rzGetType(xxx);
			if (fType == 1) {  // if folder (directory)
				char *xxx2 = new char[strlen("xcopy ") 
				+ strlen(xfNameOld) 
				+ strlen(" ") 
				+ strlen(xxx) 
				+ strlen(" /E")] {};
				strcpy(xxx2, "xcopy ");
				strcat(xxx2, xfNameOld);
				strcat(xxx2, " ");
				strcat(xxx2, xxx);
				strcat(xxx2, " /E");
				system(xxx2);
				system("D");
//				system("xcopy C:\\temp C:\\temp2 /E");
			}
			else {  // if file
				rename(this->xfPathOld, xxx);
			}
		} else {  // if copy
			char *xxx = new char[strlen(rzGetPath())
				+ strlen(delimetrff)
				+ strlen(this->xfNameOld) + 1]{};
			strcpy(xxx, rzGetPath());
			strcat(xxx, delimetrff);
			strcat(xxx, this->xfNameOld);
			rzFileCopy(this->xfPathOld, xxx);
		}
	}
	else if (m_index == 3) { //delete
		char *xxx = new char[strlen(rzGetPath())
			+ strlen(delimetrff)
			+ strlen(rzGetListName(rzGetListIndex())) + 1]{};
		strcpy(xxx, rzGetPath());
		strcat(xxx, delimetrff);
		strcat(xxx, rzGetListName(rzGetListIndex()));
		if (remove(xxx) != 0) {
//			perror("Error deleting file");
			RemoveDirectory(xxx);
		}
		//	puts("File successfully deleted");
		rzPrintForm(15, 15, 10, 10, 14, 1, 1, 1, "File deleted");
		rzSetListIndex(rzGetListIndex() - 1);
		_getch();
//		CreateFileList();
	}
	else if (m_index == 4) { //rename 
		char *OldFileFullPath = new char[strlen(rzGetPath()) + strlen(delimetrff) + strlen(rzfm_fList[rzGetListIndex()].fName) + 1]{};
		strcpy(OldFileFullPath, rzGetPath());
		strcat(OldFileFullPath, delimetrff);
		strcat(OldFileFullPath, rzfm_fList[rzGetListIndex()].fName);

		rzPrintForm(X, Y + 5, 55, 7, showStrLen, 1, 1, 1, "");
		char *xfNameNew = rzEnterText();
		char *NewFileFullPath = new char[strlen(rzGetPath()) + strlen(delimetrff) + strlen(xfNameNew) + 1] {};
		strcpy(NewFileFullPath, rzGetPath());
		strcat(NewFileFullPath, delimetrff);
		strcat(NewFileFullPath, xfNameNew);
		int result = rename(OldFileFullPath, NewFileFullPath);
		if (result == 0)
			puts("File successfully renamed");
		else
			perror("Error renaming file");
	}
	else if (m_index == 5) { //properties

	}
	else if (m_index == 6) { //create file
		rzPrintForm(X, Y+5, 55, 7, showStrLen, 1, 1, 1, "");
		char *fileName = rzEnterText(showStrLen);
		char *xxx = new char[strlen(rzGetPath())
			+ strlen(delimetrff)
			+ strlen(fileName)
			+ 1] {};
		strcpy(xxx, rzGetPath());
		strcat(xxx, delimetrff);
		strcat(xxx, fileName);
		ofstream outfile(xxx);
		if (fileName) delete[] fileName;
		if (xxx) delete[] xxx;
	}
	else if (m_index == 7) { //create directory
		rzPrintForm(5, 5, 55, 55, 20, 1, 1, 1, "");
		char *folderName = rzEnterText();
		char *xxx = new char[strlen(rzGetPath())
			+ strlen(delimetrff)
			+ strlen(folderName)
			+ 1] {};
		strcpy(xxx, rzGetPath());
		strcat(xxx, delimetrff);
		strcat(xxx, folderName);
		if (folderName) delete[] folderName;
		if (xxx) delete[] xxx;
	}

	SetCoord(X, Y + 1 + xheigth);
	SetColor(default_color);
	return m_index;
}

const void FolderFileList::rzShowTxtFile(const char *filename) {
	string line;
	ifstream myfile(filename);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			cout << line << '\n';
		}
		myfile.close();
	}
	else cout << "Unable to open file";
	short rzKey = _getch();
/*	if (rzKey == 27) {
		system("cls");
		rzPrintMenyu();
		rzPrintList();
		cout << endl;
		rzPrintMenyu();
		rzFolderInfo();
	}*/
}

char* const FolderFileList::rzInsertFileName() {
	char *file_name = new char[232]{};
	rzPrintForm(0, 0, 15, 81, 15, 1, 1, 0, "");
	SetCoord(2, 1);
	SetColor(81);
	char *temp = rzEnterNumber();
	return file_name;
};

const void FolderFileList::rzPrintForm(const short X,
	const short Y,
	const short zcolor,
	const short zcolorhead,
	const short zwidth,
	const short zheigth,
	const short zleftindent,
	const short ztopindent,
	const char  *zheadtext) {
	short xheigth = zheigth + 1;
	setlocale(LC_ALL, "C");
	SetCoord(X, Y);
	SetColor(zcolor);
	cout << char(201);
	for (short i = 0; i < zwidth; i++) { cout << char(205); }
	cout << char(187);

	for (short j = 1; j < xheigth; j++) {
		SetCoord(X, Y + j);
		SetColor(zcolor);
		cout << char(186);
		for (short i = 0; i < zwidth; i++) {
			cout << ' ';
		}
		cout << char(186);
		if (j == 1) {
			SetCoord(X + 1, Y + 1);
			SetColor(zcolorhead);
			for (short i = 0; i < zwidth; i++) {
				cout << ' ';
			}
			continue;
		}
	}

	SetCoord(X, Y + xheigth);
	SetColor(zcolor);
	cout << char(200);
	for (short i = 0; i < zwidth; i++) { cout << char(205); }
	cout << char(188);

	SetCoord(X + 2, Y + 1);
	SetColor(zcolorhead);
	cout << zheadtext;

	SetColor(default_color);
}

const void FolderFileList::rzVirtLoadWindows(const short coordX, const short coordY,
	const short sleepInt, const short colorInt) {
	for (short i = 0; i <= 100; i++) {
		SetCoord(coordX, coordY);
		cout << "Поиск информации о накопителях...";
		SetCoord(coordX + 2, coordY + 1);
		SetColor(colorInt);
		if (i <= 5) {
			cout << " ";
			SetColor(default_color);
			cout << "                   ";
		}
		else if (i <= 10) {
			cout << "  ";
			SetColor(default_color);
			cout << "                  ";
		}
		else if (i <= 15) {
			cout << "   ";
			SetColor(default_color);
			cout << "                 ";
		}
		else if (i <= 20) {
			cout << "    ";
			SetColor(default_color);
			cout << "                ";
		}
		else if (i <= 25) {
			cout << "     ";
			SetColor(default_color);
			cout << "               ";
		}
		else if (i <= 30) {
			cout << "      ";
			SetColor(default_color);
			cout << "              ";
		}
		else if (i <= 35) {
			cout << "       ";
			SetColor(default_color);
			cout << "             ";
		}
		else if (i <= 40) {
			cout << "        ";
			SetColor(default_color);
			cout << "            ";
		}
		else if (i <= 45) {
			cout << "         ";
			SetColor(default_color);
			cout << "           ";
		}
		else if (i <= 50) {
			cout << "          ";
			SetColor(default_color);
			cout << "          ";
		}
		else if (i <= 55) {
			cout << "           ";
			SetColor(default_color);
			cout << "         ";
		}
		else if (i <= 60) {
			cout << "            ";
			SetColor(default_color);
			cout << "        ";
		}
		else if (i <= 65) {
			cout << "             ";
			SetColor(default_color);
			cout << "       ";
		}
		else if (i <= 70) {
			cout << "              ";
			SetColor(default_color);
			cout << "      ";
		}
		else if (i <= 75) {
			cout << "               ";
			SetColor(default_color);
			cout << "     ";
		}
		else if (i <= 80) {
			cout << "                ";
			SetColor(default_color);
			cout << "    ";
		}
		else if (i <= 85) {
			cout << "                 ";
			SetColor(default_color);
			cout << "   ";
		}
		else if (i <= 90) {
			cout << "                  ";
			SetColor(default_color);
			cout << "  ";
		}
		else if (i <= 95) {
			cout << "                   ";
			SetColor(default_color);
			cout << " ";
		}
		else if (i <= 100) {
			cout << "                    ";
			SetColor(default_color);
			cout << "";
		}
		cout << " " << i << "%";
		Sleep(sleepInt);
		SetColor(default_color);
	}
	SetCoord(coordX, coordY);
	cout << "                                 ";
	SetCoord(coordX + 2, coordY + 1);
	cout << "                         ";
	SetCoord(0, 0);
}

const void FolderFileList::rzVirtLoad(const short coordX, const short coordY,
	const short sleepInt, const short colorInt) {
	SetCoord(coordX, coordY);
	cout << "Поиск информации о файлах и папках...";
	setlocale(LC_ALL, "C");


	char *rz_intro = new char[21]{};
	rz_intro[0] = char(178);
	rz_intro[20] = '\0';
	for (short i = 0; i <= 100; i++) {
		SetCoord(coordX + 2, coordY + 1);

		//ERROR
		short xxx = (i / 5) - 1;

		rz_intro[xxx] = char(178);
		cout << rz_intro << " " << i << "%";
		Sleep(sleepInt);
	}

	SetColor(default_color);


	//	if (rz_intro) delete[] rz_intro;

	SetCoord(coordX, coordY);
	cout << "                                     ";
	SetCoord(coordX + 2, coordY + 1);
	cout << "                         ";
	SetCoord(0, 0);

}

const void FolderFileList::rzIntro() {
	setlocale(LC_ALL, "Russian");
	SetCoord(3, 1);
	cout << "        Оставь свой след      \n";
	cout << ">> .....ooooO.................\n";
	cout << ">> .....(....)......Ooooo.....\n";
	cout << ">> ......\\..(......(....)....\n";
	cout << ">> .......\\_).......)../.....\n";
	cout << ">> .................(_/.......\n";
	Sleep(500);

	rzVirtLoadWindows();

	rzVirtLoad();
}

const void FolderFileList::rzFindFoldersFilesCount() {
	hFind = FindFirstFile(rzGetFullPathToAllFiles(), &this->FindFileData);
	this->l_count = 0;
	if (hFind != INVALID_HANDLE_VALUE) {
		do { //errorx . ..
			 //			char tempx[3]{};
			 //			if (this->xCount == 0 || this->xCount == 1) {
			 //				strcpy(tempx, FindFileData.cFileName);
			 //			}
			 //			cout << tempx << endl;
			 //			if (tempx != ".") 
			this->l_count++;
		} while (FindNextFile(hFind, &this->FindFileData));
		FindClose(hFind);
	}
	else {
		this->l_count = 1;
	}
}

const void FolderFileList::rzCreateFileList() {
	rzFindFoldersFilesCount();
	hFind = FindFirstFile(rzGetFullPathToAllFiles(), &this->FindFileData);
	if (rzfm_fList) {
//		for (int i = 0; i < rzGetCount(); i++) {
			//			if (rzfm_fList[i].fPath) delete[] rzfm_fList[i].fPath;
			//			if (rzfm_fList[i].fName) delete[] rzfm_fList[i].fName;
//		}
		delete[] rzfm_fList;
	}
	rzfm_fList = new fList[rzGetCount()];
	for (int i = 0; i < rzGetCount(); i++) {
		if (!rzfm_fList[i].fPath) rzfm_fList[i].fPath = nullptr;
		if (!rzfm_fList[i].fName) rzfm_fList[i].fPath = nullptr;
	}
	int rzfm_xCount = 0;
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (rzfm_xCount == 0) {
				if (rzfm_fList[rzfm_xCount].fPath) delete[] rzfm_fList[rzfm_xCount].fPath;
				rzfm_fList[rzfm_xCount].fPath = new char[strlen(this->xfPath) + 1]{};
				strcpy(rzfm_fList[rzfm_xCount].fPath, this->xfPath);

				if (rzfm_fList[rzfm_xCount].fName) delete[] rzfm_fList[rzfm_xCount].fName;
				rzfm_fList[rzfm_xCount].fName = new char[strlen(allff) + 1]{};
				strcpy(rzfm_fList[rzfm_xCount].fName, allff);
			}
			else {
				if (rzfm_fList[rzfm_xCount].fName) delete[] rzfm_fList[rzfm_xCount].fName;
				rzfm_fList[rzfm_xCount].fName = new char[strlen(this->FindFileData.cFileName) + 1]{};
				if (strlen(this->FindFileData.cFileName) == 1
					|| strlen(this->FindFileData.cFileName) == 2) {
					if (this->FindFileData.cFileName == "." //errorx
						|| this->FindFileData.cFileName == "..") { //errorx
						continue;
					}
				}
				strcpy(rzfm_fList[rzfm_xCount].fName, this->FindFileData.cFileName);

				if (rzfm_fList[rzfm_xCount].fPath) delete[] rzfm_fList[rzfm_xCount].fPath;
				rzfm_fList[rzfm_xCount].fPath = new char[strlen(this->xfPath) + 1]{};
				strcpy(rzfm_fList[rzfm_xCount].fPath, this->xfPath);
			}
			rzfm_xCount++;
		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}
	else {
		if (rzfm_fList[rzfm_xCount].fPath) delete[] rzfm_fList[rzfm_xCount].fPath;
		rzfm_fList[rzfm_xCount].fPath = new char[strlen(this->xfPath) + 1]{};
		strcpy(rzfm_fList[rzfm_xCount].fPath, this->xfPath);

		if (rzfm_fList[rzfm_xCount].fName) delete[] rzfm_fList[rzfm_xCount].fName;
		rzfm_fList[rzfm_xCount].fName = new char[strlen(allff) + 1]{};
		strcpy(rzfm_fList[rzfm_xCount].fName, allff);
	}
}

const void FolderFileList::rzPrintList() {
	SetCoord(0, 2);
	SetColor(default_color);
	char *filename = nullptr;
	for (int i = 0; i < this->l_count; i++) {
		if (filename) delete[] filename;
		filename = new char[strlen(this->xfPath) + strlen(allff) + strlen(rzfm_fList[i].fName) + 1]{};
		strcpy(filename, this->xfPath);
		strcat(filename, allff);
		strcat(filename, rzfm_fList[i].fName);
		if (this->l_index == i) {
			SetColor(this->l_color);
			if (rzGetType(filename) == 1) {
				cout << "Dir.: ";
			} else {
				i != 0 ? cout << "File: " : cout << "Dir.: ";
			}
			cout << rzStringConversion(rzfm_fList[i].fName, showStrLen) << endl;
//			cout << rzfm_fList[i].fName << endl;
			SetColor(default_color);
		} else {
			if (rzGetType(filename) == 1) {
				cout << "Dir.: ";
			} else {
				i != 0 ? cout << "File: " : cout << "Dir.: ";
			}
			cout << rzStringConversion(rzfm_fList[i].fName, showStrLen) << endl;
//			cout << rzfm_fList[i].fName << endl;
		}
	}
	if (filename) delete[] filename;
	SetColor(default_color);
}

char* const FolderFileList::rzStringConversion(const char *xfilename, const short xlength) {
	if (strConversion) delete[] strConversion;
	strConversion = new char[xlength+1]{};
	for (short k = 0; k < xlength+1; k++) {
		strConversion[k] = xfilename[k];
		if (k > (short)strlen(xfilename)) {
			strcat(strConversion, " ");
		}
	}
	if ((short)strlen(xfilename) > xlength) {
		strConversion[xlength - 1] = '.';
		strConversion[xlength - 2] = '.';
		strConversion[xlength - 3] = '.';
	}
	strConversion[xlength] = '\0';
	return strConversion;
}

const void FolderFileList::rzFileCopy(const char *old_filename_path,
	const char *new_filename_path) {
	if (rzGetType(old_filename_path) == 0) {
		//	clock_t start, end;
		//	start = clock();

		/*
		inline bool exists_test0 (const std::string& name) {
		ifstream f(name.c_str());
		return f.good();
		}
		inline bool exists_test1 (const std::string& name) {
		if (FILE *file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
		} else {
		return false;
		}
		}
		inline bool exists_test2 (const std::string& name) {
		return ( access( name.c_str(), F_OK ) != -1 );
		}
		inline bool exists_test3 (const std::string& name) {
		struct stat buffer;
		return (stat (name.c_str(), &buffer) == 0);
		}
		//	Method exists_test0 (ifstream): **0.485s**
		//	Method exists_test1 (FILE fopen): **0.302s**
		//	Method exists_test2 (posix access()): **0.202s**
		//	Method exists_test3 (posix stat()): **0.134s**
		*/
		string name = new_filename_path;
		ifstream f(name.c_str());
		if (f.good() == 1) {
			short z_x = 19, z_y = 9;
			rzPrintForm(z_x, z_y, 55, 81, 12, 3, 1, 2, "File exist");
			SetCoord(z_x + 1, z_y + 2);
			SetColor(55);
			cout << "  Replace?";
			SetColor(default_color);
			string rz_yesno[]{ "YES", "NO" };
			short xxx = 0, yn_index = 0;
			do {
				for (short i = 0; i < 2; i++) {
					if (yn_index == i) {
						SetColor(55);
						SetCoord(z_x + 3, z_y + 3);
						cout << rz_yesno[0];
						SetColor(70);
						SetCoord(z_x + 8, z_y + 3);
						cout << rz_yesno[1];
					}
					else {
						SetCoord(z_x + 3, z_y + 3);
						cout << rz_yesno[0];
						SetColor(55);
						SetCoord(z_x + 8, z_y + 3);
						cout << rz_yesno[1];
						SetColor(70);
					}
				}
				do {
					xxx = _getch();
				} while (xxx == 224);
				if (xxx == 77) {
					yn_index++;
					if (yn_index > 2) yn_index = 1;
				}
				if (xxx == 75) {
					yn_index--;
					if (yn_index < 0) yn_index = 0;
				}
				if (xxx == 13) {
					if (yn_index == 0) {
						ifstream source(old_filename_path, ios::binary);
						ofstream dest(new_filename_path, ios::binary);
						// file size
						source.seekg(0, ios::end);
						ifstream::pos_type rz_size = source.tellg();
						source.seekg(0);
						// allocate memory for buffer
						char *z_buffer = new char[(int)rz_size + 1]{};
						// copy file
						source.read(z_buffer, rz_size);
						dest.write(z_buffer, rz_size);
						// clean up
						delete[] z_buffer;
						source.close();
						dest.close();
						break;
					}
					else {
						break;
					}
				}
				if (xxx == 27) break;
			} while (true);
		}
		else {
			ifstream source(old_filename_path, ios::binary);
			ofstream dest(new_filename_path, ios::binary);
			// file size
			source.seekg(0, ios::end);
			ifstream::pos_type size = source.tellg();
			source.seekg(0);
			// allocate memory for buffer
			char *z_buffer = new char[(int)size + 1]{};
			// copy file
			source.read(z_buffer, size);
			dest.write(z_buffer, size);
			// clean up
			delete[] z_buffer;
			source.close();
			dest.close();
			//		end = clock();
			/*
			cout << "CLOCKS_PER_SEC " << CLOCKS_PER_SEC << "\n";
			cout << "CPU-TIME START " << start << "\n";
			cout << "CPU-TIME END " << end << "\n";
			cout << "CPU-TIME END - START " << end - start << "\n";
			cout << "TIME(SEC) " << static_cast<double>(end - start) / CLOCKS_PER_SEC << "\n";
			*/
		}
	}
	else {
		char *rz_command = new char[strlen("copy ") + strlen(old_filename_path) + strlen(new_filename_path) + 2]{};
		strcpy(rz_command, "copy ");
		//strcat(rz_command, old_filename_path);
		strcat(rz_command, "C\\test");
		strcat(rz_command, " ");
		//strcat(rz_command, new_filename_path);
		strcat(rz_command, "D\\test");
		system(rz_command);
	}
	/*
	char filename[232];
	cin.getline(filename, 232);
	int a = CopyFile(filename, "D:\\123.txt", true);
	if (a) cout << "file copyed" << endl;
	else cout << "file is not copy" << endl;
	*/
}

char* const FolderFileList::rzEnterNumber(const short charlen, bool telnumber) {
	short xcharlen = charlen < 13 ? 13 : charlen;
	short xchar;
	char *xstring = new char[xcharlen] {};
	char xtemp[2]{};
	while (true) {
		xchar = _getch();
		xtemp[0] = (char)xchar;
		xtemp[1] = '\0';
		if (xchar == 43 && telnumber) {
			if (strlen(xstring) == 0) {
				cout << xtemp;
				strcat(xstring, xtemp);
			}
		}
		else if ((xchar >= 48 && xchar <= 57)) {
			if (short(strlen(xstring)) < xcharlen) {
				cout << xtemp;
				strcat(xstring, xtemp);
			}
		}
		else if (xchar == 8) {
			if (strlen(xstring) > 0) {
				cout << '\b' << ' ' << '\b';
				xstring[strlen(xstring) - 1] = '\0';
			}
		}
		else if (xstring[0] == '0' && xstring[1] == '0') {
			cout << '\b' << ' ' << '\b' << '\b' << ' ' << '\b';
			xstring[0] = '+';
			xstring[1] = '\0';
			cout << xstring;
		}
		else if (xchar == 13) break;
	}
	SetColor(default_color);
	return xstring;
}

const void FolderFileList::rzPassEnter() {
	size_t lenFullPathEnter = (strlen(rzGetListPath(rzGetListIndex()))
		+ strlen(delimetrff)
		+ strlen(rzGetListName(rzGetListIndex()))
		+ strlen(delimetrff)
		+ strlen(allff)
		+ 1);
	char *FullPathEnter = new char[lenFullPathEnter] {};
	strcpy(FullPathEnter, rzGetListPath(rzGetListIndex()));
	strcat(FullPathEnter, delimetrff);
	strcat(FullPathEnter, rzGetListName(rzGetListIndex()));
	strcat(FullPathEnter, delimetrff);
	strcat(FullPathEnter, allff);

	int xlenFullPathEnter = ((int)strlen(rzGetListPath(rzGetListIndex()))
		+ (int)strlen(delimetrff)
		+ (int)strlen(rzGetListName(rzGetListIndex())) + (int)1);
	char *xFullPathEnter = new char[xlenFullPathEnter] {};
	strcpy(xFullPathEnter, rzGetListPath(rzGetListIndex()));
	strcat(xFullPathEnter, delimetrff);
	strcat(xFullPathEnter, rzGetListName(rzGetListIndex()));

	if (rzGetListIndex() == 0) {};
	rzSetListPath(xFullPathEnter);
	rzCreateFileList();
	if (FullPathEnter) delete[] FullPathEnter;
	if (xFullPathEnter) delete[] xFullPathEnter;
}

const void FolderFileList::rzPassBack() {
//	if (strlen(rzGetPath()) > 2) {
		char *backPuth = new char[strlen(rzGetFullPathToAllFiles()) + 1]{};
		strcpy(backPuth, rzGetFullPathToAllFiles());
		short rzcount = 0;
		for (short i = short(strlen(rzGetFullPathToAllFiles())); i >= 0; i--) {
			if (rzcount == 2) {
				break;
			}
			if (backPuth[i] == *delimetrff) {
				rzcount++;
			}
			backPuth[i] = '\0';
		}
		rzSetListPath(backPuth);
		if (backPuth) delete[] backPuth;
		rzCreateFileList();
//	}
}

const void FolderFileList::rzFolderInfo() {
	SetColor(100);
	cout << endl << rzGetCount() - 1 << " items";
	SetColor(default_color);
	cout << " ";

	if (rzGetListIndex() != 0) {
		SetColor(100);
		rzGetCount() > 0 ? cout << "1" : cout << "0";
		cout << " item selected";
		SetColor(default_color);
		cout << " ";

		SetColor(100);
		//*************************
	
		char *rzTempFile = new char[strlen(rzGetPath()) + strlen(delimetrff) + strlen(rzfm_fList[rzGetListIndex()].fName) + 1]{};
		strcpy(rzTempFile, rzGetPath());
		strcat(rzTempFile, delimetrff);
		strcat(rzTempFile, rzfm_fList[rzGetListIndex()].fName);
		struct stat fi;
		stat(rzTempFile, &fi);
		if (fi.st_size > 0) { printf("file size: %d bytes", fi.st_size); }
		else {
			fstream file(rzTempFile);
			//	DWORD size = 0;
			file.seekg(0, std::ios::end);
			//	size = file.tellg();
			setlocale(LC_ALL, "Russian");
			if (DWORD(file.tellg()) > 0) {
				printf("file size: %d bytes", DWORD(file.tellg()) <= 0 ? DWORD(0) : DWORD(file.tellg()));
			}
			file.close();
		}
		/*
		long nFileLen = 0;
		struct _stat st;
		int res = _stat("somefile.name", &st);
		if (res == 0) nFileLen = st.st_size;
		if (nFileLen > 0) printf("file size: %d bytes", nFileLen);
		*/
		if (rzTempFile) delete[] rzTempFile;
		SetColor(default_color);
		cout << " ";
		cout << endl;
	}
}

const short FolderFileList::rzGetType(const char *fullPath, bool visible) {
	DWORD d = GetFileAttributes(fullPath);
	//9 - Invalid attributes 
	//0 - Error
	//1 - Directory
	//2 - Archive
	//3 - Compressed
	//4 - Hidden
	//5 - Read-only
	//	cout << d;
	//	std::cout << "Attributes for file: ";
	//	fputws(filename, stdout);
	if (d == INVALID_FILE_ATTRIBUTES) {
		if (visible) std::cout << "Invalid attributes" << std::endl;
		if (!visible) return 9;
	}
	else {
		if (d & FILE_ATTRIBUTE_ARCHIVE) {
			if (visible) std::cout << "Archive" << std::endl;
			d &= ~FILE_ATTRIBUTE_ARCHIVE;
			if (!visible) return 2;
		}
		if (d & FILE_ATTRIBUTE_COMPRESSED) {
			if (visible) std::cout << "Compressed" << std::endl;
			d &= ~FILE_ATTRIBUTE_COMPRESSED;
			if (!visible) return 3;
		}
		if (d & FILE_ATTRIBUTE_DIRECTORY) {
			if (visible) std::cout << "Directory" << std::endl;
			d &= ~FILE_ATTRIBUTE_DIRECTORY;
			if (!visible) return 1;
		}
		if (d & FILE_ATTRIBUTE_HIDDEN) {
			if (visible) std::cout << "Hidden" << std::endl;
			d &= ~FILE_ATTRIBUTE_HIDDEN;
			if (!visible) return 4;
		}
		if (d & FILE_ATTRIBUTE_READONLY) {
			if (visible) std::cout << "Read-only" << std::endl;
			d &= ~FILE_ATTRIBUTE_READONLY;
			if (!visible) return 5;
		}
		if (d != 0) {
			if (visible) std::cout << "Other: " << std::hex << std::showbase << std::setw(0) << std::setfill('0') << d << std::endl;
		}
	}
	return 0;
}

const void FolderFileList::rzGetLogicalDisk() {
	system("cls");
	short countLogicDrive = 0;
	short d_index = 0;

	while (true) {
		DWORD drives = GetLogicalDrives();
		for (short i = 0; i < 26; i++) {
			if ((drives & (1 << i))) {
				countLogicDrive++;
			}
		}
		LogicDriveList = new char*[countLogicDrive] {};
		for (short i = 0; i < countLogicDrive; i++) {
			LogicDriveList[i] = new char[3]{};
		}
		countLogicDrive = 0;
		for (int i = 0; i < 26; i++) {
			if ((drives & (1 << i))) {
				TCHAR driveName[3] = { TEXT('A') + i, TEXT(':'), TEXT('\0') }; //TEXT('\\'),
				strcpy(LogicDriveList[countLogicDrive++], driveName);
			}
		}

		for (short i = 0; i < countLogicDrive; i++) {
			SetCoord(3, 3 + i);
			if (d_index == i) {
				SetColor(55);
				cout << LogicDriveList[i] << endl;
				SetColor(default_color);
			}
			else {
				cout << LogicDriveList[i] << endl;
			}
		}
		short xxx = 0;
		do {
			xxx = _getch();
		} while (xxx == 224);
		if (xxx == 13) {
			if (this->xfPath) delete[] this->xfPath;
			this->xfPath = new char[strlen(LogicDriveList[d_index]) + 1]{};
			strcpy(this->xfPath, LogicDriveList[d_index]);
			this->l_index = 0;
			rzCreateFileList();
			break;
		}
		if (xxx == 80) {
			d_index++;
			if (d_index > countLogicDrive - 1) d_index = countLogicDrive - 1;
		}
		if (xxx == 72) {
			d_index--;
			if (d_index < 0) d_index = 0;
		}
	}
}

char* const FolderFileList::rzEnterText(const short charlen) {
	short xcharlen = charlen < 13 ? 13 : charlen;
	short xchar;
	char *xstring = new char[xcharlen] {};
	char xtemp[2]{};
	while (true) {
		xchar = _getch();
		xtemp[0] = (char)xchar;
		xtemp[1] = '\0';
		if (xchar >= 48 && xchar <= 57) {
			if (short(strlen(xstring)) < xcharlen) {
				cout << xtemp;
				strcat(xstring, xtemp);
			}
		}
		if (xchar >= 65 && xchar <= 90) {
			if (short(strlen(xstring)) < xcharlen) {
				cout << xtemp;
				strcat(xstring, xtemp);
			}
		}
		if (xchar >= 97 && xchar <= 122) {
			if (short(strlen(xstring)) < xcharlen) {
				cout << xtemp;
				strcat(xstring, xtemp);
			}
		}
		if (xchar >= 128 && xchar <= 175) {
			if (short(strlen(xstring)) < xcharlen) {
				cout << xtemp;
				strcat(xstring, xtemp);
			}
		}
		if (xchar >= 224 && xchar <= 239) {
			if (short(strlen(xstring)) < xcharlen) {
				cout << xtemp;
				strcat(xstring, xtemp);
			}
		}
		if (xchar == 46) {
			if (short(strlen(xstring)) < xcharlen) {
				cout << xtemp;
				strcat(xstring, xtemp);
			}
		}
		else if (xchar == 8) {
			if (strlen(xstring) > 0) {
				cout << '\b' << ' ' << '\b';
				xstring[strlen(xstring) - 1] = '\0';
			}
		}
		else if (xchar == 27) {
			char *xxx = new char[2] { " " };
			return xxx;
		}
		else if (xchar == 13) break;
	}
	SetColor(default_color);
	return xstring;
}