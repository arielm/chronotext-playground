
package org.chronotext.TestBed2;

import org.chronotext.cinder.BridgeListener;
import org.chronotext.cinder.CinderBridge;
import org.chronotext.gl.GLView;
import org.chronotext.utils.Utils;

import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity implements BridgeListener
{
  static
  {
    Utils.DEBUG = true;
    Utils.TAG = "cinder";

    System.loadLibrary("fmod");
    System.loadLibrary("TestBed2");
  }

  CinderBridge bridge;

  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);

    bridge = new CinderBridge(this);

    /*
     * THIS WILL AFFECT THE FORTHCOMING GL-CONTEXT
     */
    bridge.setViewProperties(new GLView.Properties()
      .setEGLContextClientVersion(1)
      .setPreserveEGLContextOnPause(true));

    /*
     * THIS WILL TRIGGER A SERIES OF ASYNCHRONOUS EVENTS:
     *
     * - ATTACHMENT OF THE UNDERLYING GLSurfaceView TO THE VIEW-HIERARCHY
     * - RENDERER'S THREAD LAUNCH
     * - GL-CONTEXT CREATION
     * - ETC.
     */
    setContentView(bridge.getView());
  }

  // ---

  @Override
  public void handleEvent(int eventId)
  {
    switch (eventId)
    {
      case CinderBridge.SKETCH_DID_INIT:
      {
        /*
         * EXPLICITELY INITIATING MIXPANEL ON THE MAIN-THREAD
         */
//      MixpanelAPI.getInstance(this, MIXPANEL_TOKEN);
        break;
      }

      case CinderBridge.SKETCH_DID_UNINIT:
      {
        /*
         * EXPLICITELY UN-INITIATING MIXPANEL ON THE MAIN-THREAD
         */
//      MixpanelAPI.getInstance(this, MIXPANEL_TOKEN).flush();
        break;
      }

      case CinderBridge.SKETCH_DID_SETUP:
      {
        /*
         * - IN SYNC WITH THE CinderSketch::setup CALLBACK ON THE C++ SIDE
         * - WE'RE ON THE RENDERER'S THREAD
         * - GL-CONTEXT HAS JUST BEEN CREATED
         */
        break;
      }

      case CinderBridge.SKETCH_WILL_SHUTDOWN:
      {
        /*
         * - IN SYNC WITH THE CinderSketch::shutdown CALLBACK ON THE C++ SIDE
         * - WE'RE ON THE RENDERER'S THREAD
         * - GL-CONTEXT IS ABOUT TO BE DESTROYED
         */
        break;
      }

      case CinderBridge.VIEW_WILL_APPEAR:
      case CinderBridge.APP_DID_RESUME:
      {
        /*
         * - PROPERLY FILTERED (SO MANY -- OFTEN-SPURIOUS -- SYSTEM-EVENTS TAKE PLACE ON ANDROID)
         * - IN SYNC WITH THE CinderSketch::start CALLBACK ON THE C++ SIDE
         * - WE'RE ON THE MAIN-THREAD
         */
        break;
      }

      case CinderBridge.VIEW_WILL_DISAPPEAR:
      case CinderBridge.APP_WILL_PAUSE:
      {
        /*
         * - PROPERLY FILTERED (SO MANY -- OFTEN-SPURIOUS -- SYSTEM-EVENTS TAKE PLACE ON ANDROID)
         * - IN SYNC WITH THE CinderSketch::stop CALLBACK ON THE C++ SIDE
         * - WE'RE ON THE MAIN-THREAD
         */
        break;
      }
    }
  }

  /*
   * A SYSTEM FOR RECEIVING SIMPLE MESSAGES FROM THE C++ SIDE
   * 
   * THE "SKETCH" LIVES ON THE RENDERER'S THREAD, THEREFORE THE MESSAGE
   * IS PASSING THROUGH android.os.Handler BEFORE REACHING
   * ITS DESTINATION (I.E. ON THE MAIN-THREAD)
   *
   * IN ADDITION TO THREAD SYNCHRONIZATION, THE SYSTEM IS ALSO HANDLING
   * THE "LANGUAGE BARRIER" BETWEEN JAVA AND C++
   *
   *
   * SENDING MESSAGES TO THE C++ SIDE IS POSSBILE VIA Bridge.sendMessageToSketch
   * WHICH RELIES ON boost::asio::io_service
   *
   * MESSAGE-POLLING TAKES PLACE PRIOR THE NEXT CinderSketch::update, UNLESS THE MESSAGE
   * IS SENT DURING Bridge.handleMessage(), IN WHICH CASE RE-ENTRANCY APPLIES
   */

  @Override
  public void handleMessage(int what, String body)
  {
    switch (what)
    {}
  }

  // ---

  /*
   * TODO: TRY TO SUPPRESS THE NEED FOR THE FOLLOWING "MANUAL" HOOKS
   *
   * HINT: http://developer.android.com/reference/android/app/Application.ActivityLifecycleCallbacks.html
   */

  @Override
  protected void onPause()
  {
    super.onPause();
    bridge.onPause();
  }

  @Override
  protected void onResume()
  {
    super.onResume();
    bridge.onResume();
  }

  @Override
  public void onBackPressed()
  {
    if (!bridge.onBackPressed())
    {
      super.onBackPressed();
    }
  }
}
