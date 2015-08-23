package aia;

import java.sql.Date;

public class AiaPrice {
    private long id, codeId;
    private String price;
    private Date date;
    
    public AiaPrice(long id, long codeId, String price, Date date) {
        this.id = id;
        this.codeId = codeId;
        this.price = price;
        this.date = date;
    }

    @Override
    public String toString() {
        return String.format(
                "Price[id=%d, codeId=%d, price='%s']",
                id, codeId, price);
    }

    public long getId() {
        return this.id;
    }

    public long getCodeId() {
        return this.codeId;
    }

    public String getPrice() {
        return this.price;
    }

    public Date getDate() {
        return this.date;
    }
}
