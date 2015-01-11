#pragma once

#include <windows.h>
#include <wininet.h>
#include <map>
#include <set>
#include <list>
#include "tstring.h"
#include "netfile.h"
#include "timer.h"
#include "ui.h"
#include "internetoptions.h"
#include "ftpdir.h"

#define DOWNLOAD_CANCEL_TIMEOUT 30000
#define READ_BUFFER_SIZE        1024

using namespace std;

class Downloader;

typedef void (*FinishedCallback)(Downloader *d, bool res);

class Downloader
{
public:
    Downloader();
    ~Downloader();

    void      addFile(tstring url, tstring filename, DWORDLONG size = FILE_SIZE_UNKNOWN, tstring comp = _T(""));
    void      addFtpDir(tstring url, tstring mask, tstring destdir, bool recursive, tstring comp = _T(""));
    void      addMirror(tstring url, tstring mirror);
    void      setMirrorList(Downloader *d);
    void      clearFiles();
    void      clearMirrors();
    bool      downloadFiles(bool useComponents = true);
    void      startDownload();
    void      stopDownload();
    void      pauseDownload();
    void      resumeDownload();
    DWORDLONG getFileSizes(bool useComponents = true);
    int       filesCount();
    int       ftpDirsCount();
    bool      filesDownloaded();
    bool      fileDownloaded(tstring url);
    DWORD     getLastError();
    tstring   getLastErrorStr();
    void      setComponents(tstring comp);
    void      setUi(Ui *newUi);
    void      setInternetOptions(InternetOptions opt);
    void      setFinishedCallback(FinishedCallback callback);
    void      processMessages();

    bool stopOnError;
    bool ownMsgLoop;
    bool downloadCancelled;
    bool downloadPaused;

protected:
    bool openInternet();
    bool closeInternet();
    bool downloadFile(NetFile *netFile);
    bool checkMirrors(tstring url, bool download/* or get size */);
    void updateProgress(NetFile *file);
    void updateFileName(NetFile *file);
    void updateSpeed(NetFile *file, Timer *timer);
    void updateSizeTime(NetFile *file, Timer *timer);
    void updateStatus(tstring status);
    void setMarquee(bool marquee, bool total = true);
    void storeError();
    void storeError(tstring msg, DWORD errcode = 0);
    void scanFtpDir(FtpDir *ftpDir);
    void initFtpDirs();
    tstring msg(string key);
    
    map<tstring, NetFile *>    files;
    multimap<tstring, tstring> mirrors;
    set<tstring>               components;
    list<FtpDir *>             ftpDirs;
    DWORDLONG                  filesSize;
    DWORDLONG                  downloadedFilesSize;
    HINTERNET                  internet;
    Timer                      sizeTimeTimer;
    DWORD                      errorCode;
    tstring                    errorStr;
    Ui                        *ui;
    InternetOptions            internetOptions;
    HANDLE                     downloadThread;
    FinishedCallback           finishedCallback;
    MSG                        windowsMsg;
    bool                       ftpDirsProcessed;

    friend void downloadThreadProc(void *param);
    friend class Ui;
};
