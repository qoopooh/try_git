package berm;

public class Main {
  public static void main(String[] args) {
    System.out.println();

    Joby joby = new Joby("https://direct.aia.co.th/btob/FundQueryServlet?actionType=show");
    System.out.println("Title is: " + joby.getTitle());
    joby.getTd();

    System.out.println();
  }
}

