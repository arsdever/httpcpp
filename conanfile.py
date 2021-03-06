from conans import ConanFile

class HttpConanfile(ConanFile):
	name = "http"
	version = "0.1"
	description = "C++ http server"
	license = "MIT"
	author = "Arsen Gharagyozyan (arsdever) [arsen.gharagyozyn.96@gmail.com]"
	exports = "include/*.h"
	requires = "sockpp/0.7"
	
	def package(self):
		self.copy("*.h", "include/http", "include")

	def package_info(self):
		self.cpp_info.name = "http"
		self.cpp_info.includedir = ["include"]