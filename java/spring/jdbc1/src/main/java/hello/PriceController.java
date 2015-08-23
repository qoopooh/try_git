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

import aia.AiaPrice;

@Controller
public class PriceController {

    @RequestMapping("/price")
    public String index(
            @RequestParam(value="code", required=false, defaultValue="Aia Fund") String code,
            @RequestParam(value="id", required=true) int codeId,
            Model model) {
        try {
            SimpleDriverDataSource dataSource = new SimpleDriverDataSource();
            dataSource.setDriver(new com.mysql.jdbc.Driver());
            dataSource.setUrl("jdbc:mysql://192.168.1.57/test");
            dataSource.setUsername("root");
            dataSource.setPassword("sddba");

            JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);

            String sqlSelect = "SELECT * FROM aiaprice WHERE code_id=" + codeId
                + " ORDER BY id DESC";

            List<AiaPrice> list = jdbcTemplate.query(sqlSelect, new RowMapper<AiaPrice>() {

                public AiaPrice mapRow(ResultSet result, int rowNum) throws SQLException {
                    AiaPrice price = new AiaPrice(
                            result.getLong("id"),
                            result.getLong("code_id"),
                            result.getString("price"),
                            result.getDate("date"));

                    return price;
                }
            });

            model.addAttribute("prices", list);
        } catch (SQLException e) {
        }
        model.addAttribute("code", code);
        return "price";
    }
}
