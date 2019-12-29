#include <cpprest/http_client.h>
#include <fmt/printf.h>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/freetype.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

int main()
{
  try {

    // 1.- Download JSON with background image data
    web::http::client::http_client client("http://www.bing.com");
    web::uri_builder builder("/HPImageArchive.aspx");
    builder.append_query("format", "js").append_query("idx", "0").append_query("n", "1");
    auto image_data_url = builder.to_string();
    fmt::print("URL with image info: {}\n", image_data_url);

    auto response = client.request(web::http::methods::GET, image_data_url).get(); // Note: get() make this block
    fmt::print("HPImageArchive.aspx status code: {}\n", response.status_code());
    if (response.status_code() != 200)
      return 1;
    auto json = response.extract_json().get(); // Note: get() make this block

    // 2.- Parse JSON extracting url of the current (index 0) image and its name
    auto image_0 = json["images"].at(0);
    auto copyright = image_0["copyright"].as_string();
    auto title = image_0["title"].as_string();
    auto url_image_0 = image_0["url"].as_string();
    fmt::print("URL image 0: {}\n", url_image_0);
    fmt::print("copyright: {}\n", copyright);
    fmt::print("title: {}\n", title);

    // We need to extract the "id" query component in the previous URL to use
    // it as the saved filename:
    web::uri image_uri{url_image_0};
    auto filename = web::uri::split_query(image_uri.query())["id"];
    fmt::print("Image filename: {}\n", filename);

    // 3.- Make a new query to the same server (therefore we reuse 'client') but
    // getting the image itself
    response = client.request(web::http::methods::GET, url_image_0).get(); // Note: get() make this block
    auto image_bytes = response.extract_vector().get();
    fmt::print("Content-length: {} Bytes readed: {}\n", response.headers().content_length(), image_bytes.size());

    //----------------------------------
    cv::InputArray buf{image_bytes};
    cv::Mat img = cv::imdecode(buf, cv::IMREAD_UNCHANGED);

    cv::Ptr<cv::freetype::FreeType2> ft2;
    ft2 = cv::freetype::createFreeType2();
    ft2->loadFontData("mplus-1p-regular.ttf", 0);

    int linestyleAntialiased = 16;
    
    int textFontHeight = 16;
    cv::Point posTitle{10, img.rows-35};
    int thicknessTitle = 1;
    ft2->putText(img, title, posTitle, textFontHeight, cv::Scalar::all(0), thicknessTitle, linestyleAntialiased, true);

    cv::Point posCopyright{10, img.rows - 15};
    int thicknessCopyright = -1;
    ft2->putText(img, copyright, posCopyright, textFontHeight, cv::Scalar::all(0), thicknessCopyright,
                 linestyleAntialiased, true);

    cv::Point posNumber{img.cols-60, 15};
    int numberFontHeight = 64;
    int thicknessNumber = 3;
    ft2->putText(img, "1", posNumber, numberFontHeight, cv::Scalar::all(0), thicknessNumber, linestyleAntialiased,
                 false);

    cv::imshow("Bing", img);
    cv::waitKey();

  } catch (web::http::http_exception &e) {
    fmt::print("Terminating by http_exception: {}\n", e.what());
  } catch (std::exception &e) {
    fmt::print("Terminating by std::exception: {}\n", e.what());
  } catch (...) {
    fmt::print("Terminating by unknown exception\n");
  }
}
