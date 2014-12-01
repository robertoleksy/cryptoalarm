/*
 * cRecorder.h
 *
 *  Created on: 26 lis 2014
 */

#ifndef CRECORDER_H_
#define CRECORDER_H_

#include "libs.h"
#include "cSound.h"
#include "cSoundFrame.h"
#include <boost/circular_buffer.hpp>

#define CBUFF_SIZE 200
#define STEREO 2
#define MONO 1
#define EVENT_TIME 10
#define FIRST_FILES_TIME 1
#define NEXT_FILES_TIME 10

const std::string recDirName = "recordings/";

class cAlarm {
};

class cAlarmSoundRecorder: public sf::SoundBufferRecorder {
	boost::circular_buffer<cSoundFrame> mRawBuffer = boost::circular_buffer<
			cSoundFrame>(CBUFF_SIZE);

	std::chrono::system_clock::time_point mAlarmLastTime;
	std::chrono::system_clock::duration diffToAlarm;

	bool isEvent = false;
	bool savedMinusFile = false;
	unsigned int mSavedFiles = 0;
	virtual bool OnStart() {
		std::cout << "Start sound recorder" << std::endl;
		return true;
	}

	void createAndSaveFrameToCBuff(const sf::Int16* Samples, std::size_t SamplesCount) {
		cSoundFrame mSoundFrame(Samples, SamplesCount);
		mRawBuffer.push_back(mSoundFrame);

	}

	std::vector<sf::Int16> mergeCBuff() {
		std::vector<sf::Int16> samplesFromCBuff;
		time_t tt;
		tt = std::chrono::system_clock::to_time_t(mRawBuffer.at(0).getStartTime());
		_dbg1("time " << ctime(&tt));
		for (auto sample : mRawBuffer) {
			auto chunk = sample.getSamplesVec();
			for(auto element : chunk) samplesFromCBuff.push_back(element);
		}
		return samplesFromCBuff;
	}


	/**
	 * Audio samples are provided to the onProcessSamples function every 100 ms.
	 * This is currently hard-coded into SFML and you can't change that
	 * (unless you modify SFML itself).
	 *
	 * SamplesCount = size of Samples
	 */
	virtual bool OnProcessSamples(const sf::Int16* Samples, std::size_t SamplesCount) {
		unsigned int SampleRate = GetSampleRate();
		createAndSaveFrameToCBuff(Samples, SamplesCount);

		auto sound = std::make_shared<cSound>(isEvent);
		auto wasAlarm = sound->ProccessRecording(Samples, SamplesCount, SampleRate);
		if (wasAlarm) {
			mAlarmLastTime = std::chrono::system_clock::now();
			assert(!mRawBuffer.empty());
			auto vecOfSamples = mergeCBuff();
			saveBuffToFile(vecOfSamples.data(), vecOfSamples.size(), SampleRate, sound->currentDateTime());
		}

		diffToAlarm = std::chrono::system_clock::now() - mAlarmLastTime;
		if (diffToAlarm < std::chrono::seconds(EVENT_TIME)) {
			_note("event");
			isEvent = true;
			auto vecOfSamples = mergeCBuff();
			
			// saving last 20s
			if (!savedMinusFile) {
				_dbg2("saving 20s file");
				saveBuffToFile(vecOfSamples.data(), vecOfSamples.size(), SampleRate, sound->currentDateTime());
				savedMinusFile = true;
				mRawBuffer.clear();
			}
			
			// saving 3 files (1s)
			else if (mSavedFiles < 3 && mRawBuffer.size() >= FIRST_FILES_TIME * 10) {
				_dbg2("saving 1s file");
				++mSavedFiles;
				saveBuffToFile(vecOfSamples.data(), vecOfSamples.size(), SampleRate, sound->currentDateTime());
				mRawBuffer.clear();
			}
			
			// other files (10s)
			else if (mSavedFiles >= 3 && mRawBuffer.size() >= NEXT_FILES_TIME * 10) {
				_dbg2("saving 10s file");
				saveBuffToFile(vecOfSamples.data(), vecOfSamples.size(), SampleRate, sound->currentDateTime());
				mRawBuffer.clear();
			}
			
		}
		else {
			_fact("no event");
			isEvent = false;
			savedMinusFile = false;
			mSavedFiles = 0;
		}
		// return true to continue the capture, or false to stop it
		return true;
	}

	void saveBuffToFile(const sf::Int16* Samples, std::size_t SamplesCount, unsigned int SampleRate, std::string filename) {
		assert(Samples != nullptr && SamplesCount > 0);
		_dbg3("Start save to file");
		_dbg3("size of samples: " << sizeof(Samples));
		filename += ".wav";
		sf::SoundBuffer buff;
		buff.LoadFromSamples(Samples, SamplesCount, MONO, SampleRate);
		assert(buff.GetDuration() != 0);
		_dbg2("size of samples(get samples): " << sizeof(buff.GetSamples()));
		_info("samples count: " << buff.GetSamplesCount() << ", duration: " << buff.GetDuration());
		if (!buff.SaveToFile(recDirName+filename)) _erro(filename << " not saved :( ");
		else _note("File saved " << recDirName+filename);
	}

	virtual void OnStop() {
		std::cout << "Stop sound recorder" << std::endl;
	}

};

class cRecorder {
public:
	cRecorder();
	virtual ~cRecorder();
	void startRecording();

	const bool fromMicrophoneMode;

private:
	cAlarmSoundRecorder Recorder;

	void waitForExitKey();
};

#endif /* CRECORDER_H_ */