<?php
// Đường dẫn tới file data.json
$file_path = 'data.json';

// Đặt header để đảm bảo nội dung trả về là JSON
header('Content-Type: application/json');

// Kiểm tra nếu request là phương thức POST
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    
    // Bước 1: Lấy dữ liệu JSON từ request
    $json_data = file_get_contents('php://input');

    // Bước 2: Giải mã JSON thành mảng PHP
    $data = json_decode($json_data, true);

    if ($data["code"] != "min") {
        echo json_encode([
            'status' => 'error',
            'message' => 'Không đúng mã xác thực'
        ]);
        return;
    }

    // Kiểm tra xem dữ liệu có hợp lệ không
    if (is_array($data)) {
        // Bước 3: Mã hóa dữ liệu mới thành JSON
        $new_json_data = json_encode($data["value"], JSON_PRETTY_PRINT);

        // Bước 4: Ghi đè dữ liệu mới vào file data.json
        if (file_put_contents($file_path, $new_json_data)) {
            // Trả về phản hồi thành công
            echo json_encode([
                'status' => 'success',
                'message' => 'Cập nhập thành công'
            ]);
        } else {
            // Trả về phản hồi lỗi khi ghi dữ liệu thất bại
            echo json_encode([
                'status' => 'error',
                'message' => 'Lưu dữ liệu lỗi'
            ]);
        }
    } else {
        // Trả về phản hồi lỗi khi dữ liệu không hợp lệ
        echo json_encode([
            'status' => 'error',
            'message' => 'Dữ liệu không hợp lệ'
        ]);
    }
} else {
    // Trả về phản hồi lỗi nếu không phải phương thức POST
    echo json_encode([
        'status' => 'error',
        'message' => 'Invalid request method'
    ]);
}
?>
