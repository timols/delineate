guard 'rake', :task => 'clean', :run_on_start => false do
  watch(%r{^ext/delineate/(.+)\.(rb|c|h)$})
end

guard 'rake', :task => 'compile' do
  watch(%r{^ext/delineate/(.+)\.(rb|c|h)$})
end

guard :test do
  watch(%r{^lib/(.+)\.rb$})     { |m| "test/#{m[1]}_test.rb" }
  watch(%r{^test/.+_test\.rb$})
  watch('test/test_helper.rb')  { "test" }
end
