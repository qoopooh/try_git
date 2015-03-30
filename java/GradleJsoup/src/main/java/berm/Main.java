package berm;

import java.io.IOException;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;

public class Main {
  public static void main(String[] args) throws IOException {
    Document doc = Jsoup.connect("https://direct.aia.co.th/btob/FundQueryServlet?actionType=show").get();

    //String html = "<html><head><title>First parse</title></head>"
    //+ "<body><p>Parsed HTML into a doc.</p></body></html>";
    //Document doc = Jsoup.parse(html);

    String title = doc.title();
    System.out.println();
    System.out.println("Title is: " + title);
  }
}

