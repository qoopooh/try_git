package berm;

import java.io.IOException;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

public class Joby {

  private String mUrl;
  private Document mDoc;

  public Joby() {
  }

  public Joby(String url) {
    mUrl = url;
    mDoc = load(mUrl);
  }

  public Document load(String url) {
    Document doc = null;

    try {
      doc = Jsoup.connect(url).get();
    } catch (IOException e) {

    }
    return doc;
  }

  public String getTitle() {
    return mDoc.title();
  }

  public void getTd() {
    Element body = mDoc.body();
    Elements tds = body.getElementsByTag("td");
    int count = 0;
    for (Element td : tds) {
      System.out.println("" + ++count + ": " + td.text());
    }
  }
}
