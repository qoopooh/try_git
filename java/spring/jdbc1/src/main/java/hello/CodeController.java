package hello;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.jdbc.datasource.SimpleDriverDataSource;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;

import aia.AiaCode;

@Controller
public class CodeController {

    @RequestMapping("/code")
    public String code (
            @RequestParam(value="name", required=false, defaultValue="เบิ้ม") String name,
            @RequestParam(value="age", required=false, defaultValue="0") int age,
            Model model) {
        try {
            SimpleDriverDataSource dataSource = new SimpleDriverDataSource();
            dataSource.setDriver(new com.mysql.jdbc.Driver());
            //dataSource.setUrl("jdbc:mysql://localhost/test");
            dataSource.setUrl("jdbc:mysql://192.168.1.57/test");
            dataSource.setUsername("root");
            dataSource.setPassword("sddba");

            JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);

            String sqlSelect = "SELECT * FROM aiacode";
            List<AiaCode> listCode = jdbcTemplate.query(sqlSelect, new RowMapper<AiaCode>() {

                public AiaCode mapRow(ResultSet result, int rowNum) throws SQLException {
                    AiaCode code = new AiaCode(
                            result.getLong("id"),
                            result.getString("code"),
                            result.getString("name"),
                            result.getString("price"),
                            result.getDate("date"));

                    return code;
                }
            });

            model.addAttribute("codes", listCode);
            //model.addAttribute("name", codes(listCode));
        } catch (SQLException e) {
        }
        model.addAttribute("name", name);
        model.addAttribute("age", age);
        return "code";
    }

    public String codes(List<AiaCode> list) {
        StringBuilder sb = new StringBuilder();

        for (AiaCode a : list) {
            sb.append(a).append("\n");
        }

        return sb.toString();
    }
}
