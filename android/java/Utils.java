package com.bgcottrell.explorer;
import android.app.Activity;
import android.content.Intent;
import java.io.File;
import android.net.Uri;
import android.support.v4.content.FileProvider;
import android.content.Context;
import android.widget.Toast;
import android.util.Log;
public class Utils{
  public static final String APPLICATION_ID = "com.bgcottrell.explorer";
  public static boolean foobar()
  {
      Log.i(APPLICATION_ID, "foobar");
      return true;
}
  public static boolean openFile(Activity activity, final boolean isExternal, final String url, final String mimetype) {
    try {
      Intent i = new Intent();
      i.setAction(Intent.ACTION_VIEW);
      File file = new File(url);
      if (!isExternal) {
        i.setDataAndType(Uri.fromFile(file), mimetype);
      } else {
        Uri fullUri = FileProvider.getUriForFile(activity,
        APPLICATION_ID + ".provider",
        file);
        i.setDataAndType(fullUri, mimetype);
      }
      i.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
      activity.startActivity(i);
    } catch (Exception e) {
      return false;
    }
  return true;
  }
}

