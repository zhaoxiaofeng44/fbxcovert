#include <config.h>
#include <map>
#include <vector>
#include <meshnode.h>
#include <fbxserialize.h>
#include <jsonserialize.h>

int main(int argc, char* argv[])
{
	JsonSerialize::Set("Guardt.txt", FbxSerialize::Get("Guard.FBX"));
	FbxSerialize::Set("Guardz.FBX", JsonSerialize::Get("Guardt.txt"));
	system("pause");
	
	return 0;
}

