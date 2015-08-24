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
public class IndexController {

    @RequestMapping("/")
    public String index(Model model) {
        try {
            SimpleDriverDataSource dataSource = new SimpleDriverDataSource();
            dataSource.setDriver(new com.mysql.jdbc.Driver());
            dataSource.setUrl("jdbc:mysql://192.168.1.57/test");
            dataSource.setUsername("root");
            dataSource.setPassword("sddba");

            JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);

            String sqlSelect =
            "SELECT t4.id, t4.code, t4.name, t3.price, t3.date FROM aiacode t4," +
            "    (SELECT t1.* FROM aiaprice t1" +
            "    JOIN (SELECT code_id, MAX(id) id, price, date FROM aiaprice GROUP BY code_id) t2" +
            "    ON t1.id = t2.id AND t1.code_id = t2.code_id) t3" +
            " WHERE t4.id = t3.code_id";
                ;
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
        } catch (SQLException e) {
        }
        return "index";
    }
}
