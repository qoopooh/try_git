package aia;

import java.io.UnsupportedEncodingException;

public class AiaCode {
    private long id;
    private String code, name;
    
    public AiaCode(long id, String code, String name) {
        this.id = id;
        this.code = code;
        this.name = name;
    }

    @Override
    public String toString() {
        String name = "-";
        try {
            //name = new String(this.name.getBytes("ISO-8859-11"), "UTF-8");
            //name = new String(this.name.getBytes("TIS620"), "UTF-8");
            //name = new String(this.name.getBytes("TIS620"));
            //name = new String(this.name.getBytes("ISO-8859-11"));
            //name = new String(this.name.getBytes("TIS-620"));
            //name = new String(this.name.getBytes("CP874"));
            String s = new String(this.name.getBytes(), "TIS620");
        } catch (UnsupportedEncodingException e) {
        }
        return String.format(
                "Code [id=%d, code='%s', name='%s']",
                id, code, name);
    }
}
