package aia;

public class AiaPrice {
    private long id, codeId;
    private String price;
    
    public AiaPrice(long id, long codeId, String price) {
        this.id = id;
        this.codeId = codeId;
        this.price = price;
    }

    @Override
    public String toString() {
        return String.format(
                "Price[id=%d, codeId=%d, price='%s']",
                id, codeId, price);
    }
}
