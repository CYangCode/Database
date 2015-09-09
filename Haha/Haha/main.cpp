#include <iostream>
#include <fstream>

using namespace std;
int main()
{
	FILE * fp;
	char ed = EOF;
	if (fopen_s(&fp, "d:/test.txt", "wb+") == 0) {
		fseek(fp, 2048 - 1, SEEK_SET);
		fwrite(&ed, 1, 1, fp);
		fclose(fp);
	}

	fstream finout;
	finout.open("d:/test.txt", ios::in | ios::out | ios::binary);
	if (finout.is_open()){
		int num = 2048;
		finout.seekp(0, ios_base::beg);
		finout.write((char*)&num, sizeof(int));
		if (finout.fail()){
			exit(0);
		}
		finout.close();
	}

	fstream fou;
	fou.open("d:/test.txt", ios::in | ios::out | ios::binary);
	fou.seekg(0);


	int i ;
	while (fou.read((char*)&i, sizeof(int))){
		cout << i << endl;
		system("pause");
	}
	system("pause");
}