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

            String sqlSelect = "SELECT * FROM aiacode";
            List<AiaCode> listCode = jdbcTemplate.query(sqlSelect, new RowMapper<AiaCode>() {

                public AiaCode mapRow(ResultSet result, int rowNum) throws SQLException {
                    AiaCode code = new AiaCode(
                            result.getLong("id"),
                            result.getString("code"),
                            result.getString("name"),
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
