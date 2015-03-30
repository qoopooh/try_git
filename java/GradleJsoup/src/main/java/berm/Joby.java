package berm;

import java.io.IOException;

import org.json.JSONArray;
import org.json.JSONObject;
import org.json.JSONException;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

public class Joby {

  private static final int TITLE_ROW          = 1;
  private static final int FUND_INDEX_OFFSET  = 2;

  private static final int NAME_COL           = 0;
  private static final int PRICE_COL          = 1;
  private static final int AIA_SELL_COL       = 2;
  private static final int AIA_BUY_COL        = 3;
  private static final int REF_SELL_COL       = 4;
  private static final int REF_BUY_COL        = 5;
  private static final int ASSET_COL          = 6;
  private static final int OTHER_COLS         = 7;

  private static final String JSON_NAME       = "name";
  private static final String JSON_FUND_INDEX = "fund";
  private static final String JSON_PRICE      = "price";
  private static final String JSON_PRICES     = "prices";
  private static final String JSON_AIA_SELL   = "aia_sell";
  private static final String JSON_AIA_BUY    = "aia_buy";
  private static final String JSON_REF_SELL   = "ref_sell";
  private static final String JSON_REF_BUY    = "ref_buy";
  private static final String JSON_ASSET      = "asset";

  private String mUrl;
  private Document mDoc;

  private JSONObject mJsonAll;
  private JSONObject mJsonName;
  private JSONArray mJsonPrices;

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
    int rowCount = 0;
    int colCount = 0;
    JSONObject jsonPrice = null;

    mJsonName = new JSONObject();
    mJsonPrices = new JSONArray();

    try {
      for (Element td : tds) {
        String text = td.text().trim();
        if (text.contains("กองทุน")) {
          ++rowCount;
          colCount = 0;

          if (rowCount > TITLE_ROW) {
            String fundIndex = "" + (rowCount - FUND_INDEX_OFFSET);

            mJsonName.put(fundIndex, text);
            jsonPrice = new JSONObject();
            jsonPrice.put(JSON_FUND_INDEX, fundIndex);
          }
        } else if (text.length() < 1) {
          System.out.println("zero");

          if (rowCount > TITLE_ROW)
            mJsonPrices.put(jsonPrice);
        } else {
          if (rowCount > TITLE_ROW) {
            if (colCount == PRICE_COL) {
              jsonPrice.put(JSON_PRICE, text);
            } else if (colCount == AIA_SELL_COL) {
              jsonPrice.put(JSON_AIA_SELL, text);
            } else if (colCount == AIA_BUY_COL) {
              jsonPrice.put(JSON_AIA_BUY, text);
            //} else if (colCount == REF_SELL_COL) {
              //jsonPrice.put(JSON_REF_SELL, text);
            //} else if (colCount == REF_BUY_COL) {
              //jsonPrice.put(JSON_REF_BUY, text);
            } else if (colCount == ASSET_COL) {
              jsonPrice.put(JSON_ASSET, text);
            }
          }
        }
        ++colCount;
        System.out.println("" + ++count + ": " + td.text());
      }

      mJsonAll = new JSONObject();
      mJsonAll.put(JSON_NAME, mJsonName);
      mJsonAll.put(JSON_PRICES, mJsonPrices);
    } catch (JSONException e) {
    }
    //System.out.println("name " + mJsonName.toString(2));
    //System.out.println("prices " + mJsonPrices.toString(2));
    System.out.println("json " + mJsonAll.toString(2));
  }
}
