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

        auto url_image_0 = json["images"].at(0)["url"].as_string(); //This will have something similar to: /th?id=OHR.ReindeerNorway_ES-ES0018263159_1920x1080.jpg&rf=LaDigue_1920x1080.jpg&pid=hp
        fmt::print("URL image 0: {}\n", url_image_0);
        
        //We need to extract the name of the file name in the previous URL:
        web::uri image_uri{url_image_0};
        auto filename = web::uri::split_query(image_uri.query())["id"];
        fmt::print("Image filename: {}\n", filename);
        
    } catch (web::http::http_exception &e) {
        fmt::print("Terminating by http_exception: {}\n", e.what());
    } catch (std::exception &e) {
        fmt::print("Terminating by std::exception: {}\n", e.what());
    } catch (...) {
        fmt::print("Terminating by unknown exception\n");
    }
}
