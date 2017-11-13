# initalize finish count
FINISH_COUNT = 10

# load file
file_name = ("eng.txt")
text = []
 File.open(file_name) do |f|
     f.each_line do |el|
           text << el.split(",")
      end
end

# setting which question style eng to jap or jap to eng
puts "Select game style\n 1 is eng to jap\n 0 is jap to eng"
style = STDIN.gets
flag = (style.to_i != 0)
# loop question..
count = 0
loop do
    answer = []
    ans = rand(text.length)
    puts text[ans][ flag ? 0 : 1]
    # select dammy answers
    answer << "#{ans}: #{text[ans][ flag ? 1 : 0]}"
    3.times do
        dammy = rand(text.length)
        answer << "#{dammy}: #{text[dammy][ flag ? 1 : 0]}"
    end
    answer.shuffle!

    puts answer
    res = STDIN.gets
    if res.to_i == ans
        puts "correct"
        count = count + 1
    else
        puts "incorrect.. ans is #{text[ans][ flag ? 1 : 0]}"
        count = count - 2
    end

    # accomplish to answer FINISH_COUNT?
    if count == FINISH_COUNT
        puts "today is ok!! exit this questioner"
        break
    end
end


