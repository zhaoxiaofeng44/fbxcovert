#pragma once

#include <vector>
#include <string>
#include <memory>
#include <animsample.h>
#include <algorithm>
#include <map>
#include <fbxsdk.h>

#define FRAME_24		FbxTime::eFrames24
#define FRAME_30		FbxTime::eFrames30

class AnimLayer
{
public:
	AnimLayer();

	AnimLayer(FbxImporter* importer);
	
	void SetInfo(FbxImporter* importer);

	int GetNumSamples();

	AnimSample* GetSample(int index);

	std::vector<std::string>& GetSamplesNames();

	void Clear();
private:


	int mNumSamples;
	std::vector<animsample_ptr>	mSamples;
	std::vector<std::string>	mSamplesNames;
};

