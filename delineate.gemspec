$:.push File.expand_path("../lib", __FILE__)

# Maintain your gem's version:
require "delineate/version"

# Describe your gem and declare its dependencies:
Gem::Specification.new do |s|
  s.name        = "delineate"
  s.version     = Delineate::VERSION
  s.authors     = ["Tim Olshansky"]
  s.email       = ["tim.olshansky@gmail.com"]
  s.homepage    = "http://timolshansky.com"
  s.summary     = "Ruby C Extensions for detecting lines on a page"
  s.description = "An extension using opencv to detect horizontal and vertical lines on a page"
  s.files       = Dir["lib/**/*.rb"] + Dir["ext/**/*.{c,h,rb}"] + ["README.md", "LICENCE.md"]
  s.extensions  = ['ext/delineate/extconf.rb']
  s.test_files  = Dir["test/**/*"]

  s.add_development_dependency "rake-compiler"
end
