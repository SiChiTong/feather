%module pyvulture 
 %{
 /*  Includes the header in the wrapper code */
 #include "window.hpp"
 //#include "qvulkanwindow.hpp"
 %}
 
 /*  Parse the header file to generate wrappers */
 //%include "window.hpp"

 namespace feather {

     namespace vulkan {

         class Window {
             public:
                 Window();
                 ~Window();
         };

     };

 };

/*
class QVulkanView {
    public:
        QVulkanView();
        ~QVulkanView();
};
*/

