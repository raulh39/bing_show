#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <fmt/printf.h>


int main()
{
    try
    {
        web::http::client::http_client client("http://www.bing.com");
        web::uri_builder builder("/HPImageArchive.aspx");
        builder
            .append_query("format", "js")
            .append_query("idx","0")
            .append_query("n","1");
        
        auto response =  client.request(web::http::methods::GET, builder.to_string()).get(); //Note: get() make this block
        fmt::print("Received response status code: {}\n", response.status_code());
        if(response.status_code() != 200) return 1;

        auto json = response.extract_json().get(); //Note: get() make this block
        auto url_image_0 = json["images"].at(0)["url"];
        fmt::print("URL image 0: {}\n", url_image_0.as_string());
        
    } catch (web::http::http_exception &e) {
        fmt::print("Terminating by http_exception: {}\n", e.what());
    } catch (std::exception &e) {
        fmt::print("Terminating by std::exception: {}\n", e.what());
    } catch (...) {
        fmt::print("Terminating by unknown exception\n");
    }
}
