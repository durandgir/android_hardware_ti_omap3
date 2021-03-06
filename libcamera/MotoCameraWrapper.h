#ifndef ANDROID_HARDWARE_MOTO_CAMERA_WRAPPER_H
#define ANDROID_HARDWARE_MOTO_CAMERA_WRAPPER_H

#include <utils/threads.h>
#include <ui/android_native_buffer.h>
#include <CameraHardwareInterface.h>

namespace android {

class MotoCameraWrapper : public CameraHardwareInterface {
public:
    virtual sp<IMemoryHeap> getPreviewHeap() const;
    virtual sp<IMemoryHeap> getRawHeap() const;
/*
    virtual void        setCallbacks(notify_callback notify_cb,
                                     data_callback data_cb,
                                     data_callback_timestamp data_cb_timestamp,
                                     void *user);

    virtual void        enableMsgType(int32_t msgType);
    virtual void        disableMsgType(int32_t msgType);
    virtual bool        msgTypeEnabled(int32_t msgType);

    virtual status_t    startPreview();
    virtual bool        useOverlay();
    virtual status_t    setOverlay(const sp<Overlay> &overlay);
    virtual void        stopPreview();
    virtual bool        previewEnabled();

    virtual status_t    startRecording();
    virtual void        stopRecording();
    virtual bool        recordingEnabled();
    virtual void        releaseRecordingFrame(const sp<IMemory> &mem);

    virtual status_t    autoFocus();
    virtual status_t    cancelAutoFocus();
    virtual status_t    takePicture();
    virtual status_t    cancelPicture();
    virtual status_t    dump(int fd, const Vector<String16> &args) const;
    virtual status_t    setParameters(const CameraParameters& params);
    virtual CameraParameters  getParameters() const;
    virtual status_t    sendCommand(int32_t command, int32_t arg1,
                                    int32_t arg2);
    virtual void        release();
*/
    static    sp<CameraHardwareInterface> createInstance(int cameraId);

private:
    typedef enum {
        CAM_SOC,
        CAM_BAYER
    } CameraType;

    class TorchEnableThread : public Thread {
        public:
            TorchEnableThread(MotoCameraWrapper *hw) :
                Thread(false), mHw(hw) { }
            void scheduleTorch() {
                cancelAndWait();
                run("TorchEnableThread");
            }
            void cancelAndWait() {
                mStopCondition.signal();
                requestExitAndWait();
            }
            virtual bool threadLoop() {
                mStopLock.lock();
                mStopCondition.waitRelative(mStopLock, 1000000000);
                if (!exitPending()) {
                    mHw->toggleTorchIfNeeded();
                }
                mStopLock.unlock();
                return false;
            }
        private:
            MotoCameraWrapper *mHw;
            mutable Mutex mStopLock;
            mutable Condition mStopCondition;
    };

    MotoCameraWrapper(sp<CameraHardwareInterface>& motoInterface, CameraType type);
    virtual ~MotoCameraWrapper();
/*
    static void notifyCb(int32_t msgType, int32_t ext1, int32_t ext2, void* user);
    static void dataCb(int32_t msgType, const sp<IMemory>& dataPtr, void* user);
    static void dataCbTimestamp(nsecs_t timestamp, int32_t msgType, const sp<IMemory>& dataPtr, void* user);
    void fixUpBrokenGpsLatitudeRef(const sp<IMemory>& dataPtr);
*/
    void toggleTorchIfNeeded();

    sp<CameraHardwareInterface> mMotoInterface;
    sp<TorchEnableThread> mTorchThread;
    CameraType mCameraType;
    bool mVideoMode;
    String8 mFlashMode;
/*
    notify_callback mNotifyCb;
    data_callback mDataCb;
    data_callback_timestamp mDataCbTimestamp;
    void *mCbUserData;
*/
    static wp<CameraHardwareInterface> singleton;

};

}; // namespace android

#endif
