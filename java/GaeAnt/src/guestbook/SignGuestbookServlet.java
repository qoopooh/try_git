package guestbook;

import com.google.appengine.api.datastore.DatastoreService;
import com.google.appengine.api.datastore.DatastoreServiceFactory;
import com.google.appengine.api.datastore.Entity;
import com.google.appengine.api.datastore.Key;
import com.google.appengine.api.datastore.KeyFactory;
import com.google.appengine.api.users.User;
import com.google.appengine.api.users.UserService;
import com.google.appengine.api.users.UserServiceFactory;

import java.io.IOException;
import java.util.Date;
import java.util.logging.Logger;

import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class SignGuestbookServlet extends HttpServlet {
    private static final Logger log = Logger.getLogger(SignGuestbookServlet.class.getName());

    @Override
    public void doPost(HttpServletRequest req, HttpServletResponse resp)
            throws IOException {
        UserService userService = UserServiceFactory.getUserService();
        User user = userService.getCurrentUser();

        String content = req.getParameter("content");
        if (content == null) {
            content = "(No greeting)";
        }
        if (user != null) {
            log.info("Greeting posted by user " + user.getNickname() + ": " + content);
        } else {
            log.info("Greeting posted anonymously: " + content);
        }
        resp.sendRedirect("/guestbook.jsp");
    }
}

