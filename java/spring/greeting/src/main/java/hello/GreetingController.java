package hello;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;

@Controller
public class GreetingController {

    @RequestMapping("/greeting")
    public String greeting(
            @RequestParam(value="name", required=false, defaultValue="เบิ้ม") String name,
            @RequestParam(value="age", required=false, defaultValue="0") int age,
            Model model) {
        model.addAttribute("name", name);
        model.addAttribute("age", age);
        return "greeting";
    }
}
