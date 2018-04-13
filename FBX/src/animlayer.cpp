#include <animlayer.h>
#include <log.h>
#include <fbxsdk.h>

AnimLayer::AnimLayer()
	: mNumSamples(0)
{
}

AnimLayer::AnimLayer(AnimLayer& other)
	: mNumSamples(other.mNumSamples)
	, mSamples(other.mSamples)
	, mSamplesNames(other.mSamplesNames)
{
}

AnimLayer::AnimLayer(FbxImporter* importer)
	: AnimLayer()
{
	SetInfo(importer);
}

void AnimLayer::SetInfo(FbxImporter* importer)
{
	mNumSamples = 0;
	for (int i = 0; i < importer->GetAnimStackCount(); ++i)
	{
		auto* Take = importer->GetTakeInfo(i);
		animsample_ptr Sample = std::make_shared<AnimSample>();
		
		FbxTime Start = Take->mLocalTimeSpan.GetStart();
		FbxTime End = Take->mLocalTimeSpan.GetStop();
		FbxTime Duration = Take->mLocalTimeSpan.GetDuration();
		FbxTime Length =  End.GetFrameCount(FRAME_24) - Start.GetFrameCount(FRAME_24) + 1;
		
		Sample->mName = Take->mName.Buffer();
		Sample->mStart = Start.Get();
		Sample->mEnd = End.Get();
		Sample->mDuration = (End.Get() - Start.Get());

		Sample->mFrameStart = Start.GetFrameCount(FRAME_24);
		Sample->mFrameEnd = End.GetFrameCount(FRAME_24);
		Sample->mFrameCount = Length.Get();
		Sample->mIndex = mNumSamples;

		auto Mode = Start.GetGlobalTimeMode();
		Sample->mFps = Start.GetFrameRate(Mode);

		
		mSamples.push_back(Sample);
		mSamplesNames.push_back(Sample->mName);
		mNumSamples++;
	}

}

int AnimLayer::GetNumSamples()
{
	return mNumSamples;
}


AnimSample* AnimLayer::GetSample(int index)
{
	return mSamples[index].get();
}


std::vector<std::string>& AnimLayer::GetSamplesNames()
{
	return mSamplesNames;
}


void AnimLayer::Clear()
{
	mNumSamples = 0;
	mSamples.clear();
	mSamplesNames.clear();
}
