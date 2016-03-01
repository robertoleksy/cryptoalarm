/*
 * cSend.cpp
 *
 *  Created on: 28 lis 2014
 *     
 */

#include "cSend.h"
#include "cFile.h"
#define SEND_LOG "send_log"

using namespace std;

const string cSend::sendScript = "cryptoalarm/soundsimple/scripts/send.py ";
bool cSend::simulationMode = false;

cSend::cSend()
{
	// TODO Auto-generated constructor stub

}

cSend::~cSend()
{
	// TODO Auto-generated destructor stub
}

void cSend::alarmHandler() {
	while (true) {
		if (cSound::alarmsToSend.empty()) sleep(1); // nothing to send
		else {
			_scope_fact("sending xmpp message from queue");
			_fact_c(SEND_LOG, "sending xmpp message from queue");
			cSound::mtx.lock();
			const string mess = cSound::alarmsToSend.top().first;
			const auto method = cSound::alarmsToSend.top().second;
			cSound::alarmsToSend.pop();
			cSound::mtx.unlock();
			send(method, mess);
		}
	}
}

void cSend::send(cSound::sendingMethod method, const std::string &message) {
	switch (method) {
	case cSound::sendingMethod::XMPP:
		_info("sending notification via XMPP");
		_info_c(SEND_LOG, "sending notification via XMPP");
		sendXMPPNotificationMessage(message);
		break;
	case cSound::sendingMethod::MAIL:
		_note("Just noise. After saving recording mail will be send");
		_note_c(SEND_LOG, "Just noise. After saving recording mail will be send");
		break;
	}
}

void cSend::sendXMPPNotificationMessage(std::string mess) {
	_note("sending XMPP message");
	_note_c(SEND_LOG, "sending XMPP message");
	const string q = " \" ";
	const string cmd = cFile::getHomeDir() + sendScript + " xmpp " + q + mess + q;
	_info(cmd);
	_info_c(SEND_LOG, cmd);
	if(!simulationMode) std::system(cmd.c_str());
}

void cSend::sendXMPPNotificationMessageInThread(std::string mess) {
	thread sendingThread(&cSend::sendXMPPNotificationMessage, mess);
	sendingThread.detach();
}

void cSend::sendSum(std::string filename) {
	if (!boost::filesystem::exists(filename)) {
		_warn("It seems file: " << filename << " doesn't exist, but I try to send checksum");
	}
	// TODO: sholud we use crypto++ library to get this sum?
	const string checkSumScript = " $(sha512sum recordings/2014-11-28.15:37:52.wav | tr \" \" \"-\" )";
	sendXMPPNotificationMessage(checkSumScript);
}

void cSend::sendMailNotificationMessage(std::string mess, std::string rec) {
	_dbg1_c(SEND_LOG, "mess: " << mess << ", recording: " << rec);
	const string q = " \"  ";
	const string cmd = cFile::getHomeDir() + sendScript + " mail " + q + mess + q + rec;
	_dbg2(cmd);

	if(simulationMode) return;
	thread systh(&cSend::sendMailHandleErrors, cmd, 0);
	systh.detach();
}

void cSend::sendMailHandleErrors(std::string toSend, int n) {
	_scope_dbg2("sending email" << toSend);
	auto err = std::system(toSend.c_str());
	// log toSend
	if (err == 0) {
		// all ok, log this
		_dbg1_c(SEND_LOG, "All ok");
	}
	else {
		if (!n) {
			_erro_c(SEND_LOG, "Problem with sending, I try send this once again");
			sendMailHandleErrors(toSend, ++n);
		}
		else {
			_erro_c(SEND_LOG, "Can't send e-mail, I send notification via XMPP");
			sendXMPPNotificationMessageInThread(toSend);
		}
	}
}

void cSend::execute(std::string cmd) {
	if(simulationMode) return;
	thread systh(std::system, cmd.c_str());
	systh.detach();
}
