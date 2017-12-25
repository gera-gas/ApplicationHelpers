## Ruby script for cleaning source files from macros "ADD_TO_SUSPICIOUS_OBJECTS_LIST".
## Run: $ ruby clean_sources.rb

$settings = {
  :start_directory => '../Demo',
  :file_extension => ['cpp', 'h'],
  :exclude_folders => []
}

# Ruby examples method for find files.
def findfiles ( dir, name )
  list = []
  require 'find'
  Find.find(dir) do |path|
    Find.prune if [".",".."].include? path
    case name
      when String
        list << path if File.basename(path) == name
      when Regexp
        list << path if File.basename(path) =~ name
    else
      raise ArgumentError
    end
  end
  return list
end

# Recursive search of files.
$settings[:file_extension].each do |ext|
  flist = []
  findfiles( $settings[:start_directory], /\w/ ).each do |file|
  	exclude = false
  	$settings[:exclude_folders].each do |folder|
  		if file.include? folder then
  			exclude = true
  			break
  		end
  	end
    flist << file if file.split('.').last == ext and not exclude
  end
  flist.each do |file|
    f = File.new( file + '~', 'w' )
    File.open( file, 'r' ).readlines.each do |line|
      if line =~ /ADD_TO_SUSPICIOUS_OBJECTS_LIST\([\w&->]+\)\;/ then
        newline = line.gsub(/ADD_TO_SUSPICIOUS_OBJECTS_LIST\([\w&->]+\)\;/, '')
        f.puts newline if newline.strip.length > 0
      else
      	f.puts line
      end
    end
    f.close
  end
  f = File.new( 'replace_sources.sh', 'a' )
  flist.each do |file|
  	f.puts "rm -f #{file}"
  	f.puts "mv #{file}~ #{file}"
  end
  f.close
end
