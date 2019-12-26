#include <cpprest/http_client.h>
#include <fmt/printf.h>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/freetype.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main()
{
  try {
    cv::Mat img(512, 512, CV_8UC3, cv::Scalar::all(255));

    cv::Ptr<cv::freetype::FreeType2> ft2;
    ft2 = cv::freetype::createFreeType2();
    ft2->loadFontData("mplus-1p-regular.ttf", 0);

    int fontHeight = 54;
    int thickness = -1;
    int linestyleAntialiased = 16;

    ft2->putText(img, "This is freetype", cv::Point(10, 200), fontHeight,
                 cv::Scalar::all(0), thickness, linestyleAntialiased, true);

    cv::putText(img, "This is Hershey", cv::Point(10, img.rows / 2),
                cv::FONT_HERSHEY_DUPLEX, 2.0, CV_RGB(118, 185, 0), 2,
                linestyleAntialiased);

    cv::imshow("Hello!", img);
    cv::waitKey();

    web::http::client::http_client client("http://www.bing.com");
    web::uri_builder builder("/HPImageArchive.aspx");
    builder.append_query("format", "js")
        .append_query("idx", "0")
        .append_query("n", "1");

    auto response = client.request(web::http::methods::GET, builder.to_string())
                        .get(); // Note: get() make this block
    fmt::print("HPImageArchive.aspx status code: {}\n", response.status_code());
    if (response.status_code() != 200)
      return 1;

    auto json = response.extract_json().get(); // Note: get() make this block

    auto url_image_0 =
        json["images"]
            .at(0)["url"]
            .as_string(); // This will have something similar to:
                          // "/th?id=OHR.ReindeerNorway_ES-ES0018263159_1920x1080.jpg&rf=LaDigue_1920x1080.jpg&pid=hp"
    fmt::print("URL image 0: {}\n", url_image_0);

    // We need to extract the "id" query component in the previous URL to use
    // it as the saved filename:
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
