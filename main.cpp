#include <cpprest/http_client.h>
#include <fmt/printf.h>
#include <Magick++.h>

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
        fmt::print("HPImageArchive.aspx status code: {}\n", response.status_code());
        if(response.status_code() != 200) return 1;

        auto json = response.extract_json().get(); //Note: get() make this block

        auto url_image_0 = json["images"].at(0)["url"].as_string(); //This will have something similar to: "/th?id=OHR.ReindeerNorway_ES-ES0018263159_1920x1080.jpg&rf=LaDigue_1920x1080.jpg&pid=hp"
        fmt::print("URL image 0: {}\n", url_image_0);
        
        //We need to extract the "id" query component in the previous URL to use it as the saved filename:
        web::uri image_uri{url_image_0};
        auto filename = web::uri::split_query(image_uri.query())["id"];
        fmt::print("Image filename: {}\n", filename);


        Magick::Image image("http://www.bing.com" + url_image_0);
        fmt::print("Image size: {}x{}\n", image.columns(), image.rows());

    } catch (web::http::http_exception &e) {
        fmt::print("Terminating by http_exception: {}\n", e.what());
    } catch (std::exception &e) {
        fmt::print("Terminating by std::exception: {}\n", e.what());
    } catch (...) {
        fmt::print("Terminating by unknown exception\n");
    }
}
