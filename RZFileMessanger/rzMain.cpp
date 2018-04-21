#include "rzHeader.h"

int main(int argc, char **argv) {
	SetConsoleTitle("Welcome to RZ File Manager");
	//locale::global(locale(""));
	//locale l866 = locale("rus_RUS.866");
	//wcout.imbue(l866);
	//wcin.imbue(l866);

	FolderFileList rzfm;

	//	int x = rzfm.getCount();
	//	cout << x << endl;
	int x_key = 0;
	while (true) {
		do {
			do {
				x_key = _getch();
			} while (x_key == 224);
			rzfm.SetColor(140);
			cout << x_key << endl;
			rzfm.SetColor(default_color);
		} while (x_key == 224);
		if (x_key == 80) {
			rzfm.rzIncListIndex();
			rzfm.rzCreateFileList();
		}
		if (x_key == 72) {
			rzfm.rzDecListIndex();
			rzfm.rzCreateFileList();
		}
		if (x_key == 79) {
			rzfm.rzMaxListIndex();
			rzfm.rzCreateFileList();
		}
		if (x_key == 71) {
			rzfm.rzMinListIndex();
			rzfm.rzCreateFileList();
		}
		if (x_key == 83) {
			rzfm.rzCreateFileList();
		}
		if (x_key == 27) {
			rzfm.rzGetLogicalDisk();
			/*char tttemp[3]{ "C:" };
			tttemp[2] = '\0';
			rzfm.SetListPath(tttemp);
			rzfm.CreateFileList();*/
		}
		if (x_key == 62) {
			exit(0);
		}
		if (x_key == 8) {
			rzfm.rzPassBack();
		}
		if (x_key == 13) {
			char *FilePath = new char[strlen(rzfm.rzGetPath()) + strlen(delimetrff) + strlen(rzfm.rzGetListName(rzfm.rzGetListIndex())) + 1] {};
			strcpy(FilePath, rzfm.rzGetPath());
			strcat(FilePath, delimetrff);
			strcat(FilePath, rzfm.rzGetListName(rzfm.rzGetListIndex()));
			char *FileName = new char[5] {};
			strcat(FileName, &FilePath[strlen(FilePath) - 4]);
			
			if (rzfm.rzEqualityText(FileName,".txt") == 1 || rzfm.rzEqualityText(FileName, ".cpp") == 1) {
				rzfm.rzShowTxtFile(FilePath);
			} else {
				if (rzfm.rzGetListIndex() == 0) rzfm.rzPassBack();
				else rzfm.rzPassEnter();
				rzfm.rzSetListIndex(0);
			}

			if (FilePath) delete[] FilePath;
			if (FileName) delete[] FileName;
		}
		if (x_key == 64) {
			rzfm.rzOtherMenyu();
		}
		if (x_key == 145) {
			rzfm.rzMaxListIndex();
			system("cls");
			rzfm.rzPrintMenyu();
			rzfm.rzPrintList();
			cout << endl;
			rzfm.rzPrintMenyu();
			rzfm.rzFolderInfo();
		}
		if (x_key == 141) {
			rzfm.rzMinListIndex();
			system("cls");
			rzfm.rzPrintMenyu();
			rzfm.rzPrintList();
			cout << endl;
			rzfm.rzPrintMenyu();
			rzfm.rzFolderInfo();
		}
		system("cls");
		rzfm.rzPrintMenyu();
		rzfm.rzPrintList();
		cout << endl;
		rzfm.rzPrintMenyu();
		rzfm.rzFolderInfo();
	};
	cout << "\n\n\t";
	system("pause");
	return 0;
}