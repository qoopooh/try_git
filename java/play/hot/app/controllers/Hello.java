package controllers;

import play.*;
import play.mvc.*;

import views.html.*;

public class Hello extends Controller {

    public static Result index() {
        return ok("Hello Play 1");
    }
}
