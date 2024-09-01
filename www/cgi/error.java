import java.io.*;

public class cgi {
    public static void main(String[] args) {
        System.out.println("HTTP/1.0 200 OK\r");
        System.out.println("Content-type: text/html\r");
        System.out.println("Content-length: 100\r");
        System.out.println("\r");
        System.out.println("<html>");
        System.out.println("<head><title>Hello Java CGI</title></head>");
        System.out.println("<body>");
        System.out.println("<h1>Hello Java CGI Working</h1>");
        System.out.println("</body>");
        System.out.println("</html>");
        exit(1);
    }
}
