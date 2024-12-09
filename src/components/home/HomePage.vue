<template>
  <div class="container max-w-4xl p-4 mx-auto" v-if="!isDeviceDetail">
    <h1 class="mb-6 text-2xl font-bold text-center md:text-3xl">
      Ứng dụng điều khiển đa năng
    </h1>

    <div class="p-4 mb-6 bg-white rounded-lg shadow-md">
      <div class="flex gap-4 pb-5">
        <button
          class="w-auto px-4 py-2 text-white transition duration-300 ease-in-out bg-blue-500 rounded-md hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-offset-2"
          v-if="!statusBLE"
          @click="connectBLE()"
        >
          <Bluetooth class="inline-block w-4 h-4 mr-1" />
          Kết nối bluetooth
        </button>

        <button
          class="w-auto px-4 py-2 text-white transition duration-300 ease-in-out bg-green-500 rounded-md hover:bg-green-600 focus:outline-none focus:ring-2 focus:ring-green-500 focus:ring-offset-2"
          v-else
        >
          <Bluetooth class="inline-block w-4 h-4 mr-1" />
          Đã kết nối
        </button>
      </div>

      <h2 class="mb-4 text-xl font-semibold">Nhận mã IR</h2>
      <div
        class="flex flex-col space-y-2 md:flex-row md:space-y-0 md:space-x-2"
      >
        <input
          v-model="codeListenIR"
          type="text"
          placeholder="Mã IR"
          class="flex-grow px-3 py-2 border rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500"
          readonly
        />

        <button
          @click="listenIR()"
          class="w-full px-4 py-2 text-white transition duration-300 ease-in-out bg-blue-500 rounded-md md:w-auto hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-offset-2"
        >
          <SatelliteDish class="inline-block w-4 h-4 mr-2" />
          Lắng nghe
        </button>
      </div>
      <div class="mt-2 mb-6 text-sm">Trạng thái: {{ litenIRStatus }}</div>

      <h2 class="mb-4 text-xl font-semibold">Gửi mã IR</h2>
      <div
        class="flex flex-col space-y-2 md:flex-row md:space-y-0 md:space-x-2"
      >
        <input
          v-model="codeSendIR"
          type="text"
          placeholder="Nhập mã"
          class="flex-grow px-3 py-2 border rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500"
        />

        <button
          @click="sendIR(codeSendIR)"
          class="w-full px-4 py-2 text-white transition duration-300 ease-in-out bg-blue-500 rounded-md md:w-auto hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-offset-2"
        >
          <Send class="inline-block w-4 h-4 mr-2" />
          Gửi
        </button>
      </div>
    </div>

    <div class="mb-3">
      <button
        class="w-auto px-4 py-2 text-white transition duration-300 ease-in-out bg-blue-500 rounded-md hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-offset-2"
        @click="isConfirmationCodeModalOpen = true"
      >
        <CloudUpload class="inline-block w-4 h-4 mr-1" />
        Đồng bộ dữ liệu
      </button>
    </div>

    <div class="grid grid-cols-1 gap-4 sm:grid-cols-2 lg:grid-cols-3">
      <div
        v-for="device in devices"
        :key="device.id"
        class="p-4 transition duration-300 ease-in-out bg-white rounded-lg shadow-md hover:shadow-lg"
      >
        <h3 class="mb-2 text-lg font-semibold">{{ device.name }}</h3>
        <p class="mb-4 text-sm text-gray-500">Loại: {{ device.type }}</p>
        <div class="flex space-x-2">
          <button
            @click="removeDevice(device.id)"
            class="px-4 py-2 text-gray-700 transition duration-300 ease-in-out bg-gray-200 rounded-md hover:bg-gray-300 focus:ring-gray-500flex-1 focus:outline-none focus:ring-2 focus:ring-offset-2"
          >
            <Trash class="inline-block w-4 h-4 mr-2" />
            Xoá
          </button>

          <!-- 'DeviceDetail', params: { id: device.id } }" -->
          <div
            @click="
              () => {
                deviceDetailID = device.id;
                isDeviceDetail = true;
              }
            "
            class="flex-1 px-4 py-2 text-center text-white transition duration-300 ease-in-out bg-blue-500 rounded-md hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-offset-2"
          >
            Chi tiết
          </div>
        </div>
      </div>
    </div>

    <!-- Float button -->
    <button
      @click="openModal"
      class="fixed flex items-center justify-center text-white transition duration-300 ease-in-out bg-blue-500 rounded-full shadow-lg bottom-6 right-6 w-14 h-14 hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-offset-2"
    >
      <PlusCircle class="w-6 h-6" />
    </button>

    <!-- Modal -->
    <div
      v-if="isModalOpen"
      class="fixed inset-0 flex items-center justify-center p-4 bg-black bg-opacity-50"
    >
      <div class="w-full max-w-md p-6 bg-white rounded-lg">
        <h2 class="mb-4 text-xl font-semibold">Thêm thiết bị mới</h2>
        <input
          v-model="newDevice.name"
          type="text"
          placeholder="Tên thiết bị"
          class="w-full px-3 py-2 mb-2 border rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500"
        />
        <input
          v-model="newDevice.type"
          type="text"
          placeholder="Loại thiết bị"
          class="w-full px-3 py-2 mb-4 border rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500"
        />
        <div class="flex justify-end space-x-2">
          <button
            @click="closeModal"
            class="px-4 py-2 text-gray-700 transition duration-300 ease-in-out bg-gray-200 rounded-md hover:bg-gray-300 focus:outline-none focus:ring-2 focus:ring-gray-500 focus:ring-offset-2"
          >
            Hủy
          </button>
          <button
            @click="addDevice"
            class="px-4 py-2 text-white transition duration-300 ease-in-out bg-blue-500 rounded-md hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-offset-2"
          >
            Thêm
          </button>
        </div>
      </div>
    </div>

    <!-- Modal config code -->
    <div
      v-if="isConfirmationCodeModalOpen"
      class="fixed inset-0 z-50 flex items-center justify-center p-4 bg-black bg-opacity-50"
    >
      <div class="w-full max-w-md p-6 bg-white rounded-lg">
        <h2 class="mb-4 text-xl font-semibold">Nhập Mã Xác Nhận</h2>
        <input
          v-model="confirmationCode"
          type="password"
          placeholder="Nhập mã xác nhận"
          class="w-full px-3 py-2 mb-4 border rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500"
        />
        <div class="flex justify-end space-x-2">
          <button
            @click="isConfirmationCodeModalOpen = false"
            class="px-4 py-2 text-gray-700 transition duration-300 ease-in-out bg-gray-200 rounded-md hover:bg-gray-300 focus:outline-none focus:ring-2 focus:ring-gray-500 focus:ring-offset-2"
          >
            Hủy
          </button>
          <button
            @click="updateDeviceData()"
            class="px-4 py-2 text-white transition duration-300 ease-in-out bg-blue-500 rounded-md hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-offset-2"
          >
            Xác nhận
          </button>
        </div>
      </div>
    </div>
  </div>

  <DeviceDetail
    :devices="devices"
    :id="deviceDetailID"
    @updateCommand="updateCommandFn"
    @runCommand="runCommanFn"
    @close="isDeviceDetail = false"
    v-else
  />
</template>

<script setup>
import { ref, reactive } from "vue";
import {
  PlusCircle,
  Trash,
  Send,
  Bluetooth,
  CloudUpload,
  SatelliteDish,
} from "lucide-vue-next";
import API from "@/api";
import axios from "@/axios";
import DeviceDetail from "@/components/home/DeviceDetail.vue";
import { useToast } from "vue-toastification";

const devices = ref([]);
const newDevice = reactive({
  name: "",
  type: "",
});
const isModalOpen = ref(false);
const deviceDetailID = ref(null);
const isDeviceDetail = ref(false);
const isConfirmationCodeModalOpen = ref(false);
const confirmationCode = ref("");
const toast = useToast();
let bleDevice;
let characteristic;
const statusBLE = ref(false);
const codeListenIR = ref();
const codeSendIR = ref();
const litenIRStatus = ref("Tắt");

const getDeviceLocal = () => {
  const deviceData = localStorage.getItem("deviceData");
  let deviceDataJson = [];

  if (deviceData) {
    deviceDataJson = JSON.parse(deviceData);
  }

  return deviceDataJson;
};

const addDevice = () => {
  if (newDevice.name && newDevice.type) {
    const dataSave = {
      id: Date.now(),
      name: newDevice.name,
      type: newDevice.type,
      commands: [],
    };

    const deviceLocal = getDeviceLocal();
    deviceLocal.push(dataSave);
    localStorage.setItem("deviceData", JSON.stringify(deviceLocal));
    devices.value.push(dataSave);

    newDevice.name = "";
    newDevice.type = "";
    closeModal();
  }
};

const removeDevice = (id) => {
  devices.value = devices.value.filter((item) => item.id !== id);

  let deviceLocal = getDeviceLocal();
  deviceLocal = deviceLocal.filter((item) => item.id !== id);
  localStorage.setItem("deviceData", JSON.stringify(deviceLocal));
};

const openModal = () => {
  isModalOpen.value = true;
};

const closeModal = () => {
  isModalOpen.value = false;
  newDevice.name = "";
  newDevice.type = "";
};

const getDeviceData = async () => {
  try {
    const res = await axios.get(API.DATA);
    const data = res.data;

    const idsDeviceLocal = getDeviceLocal().map((item) => item.id); // Lấy id ở local
    const devicesData = data.filter(
      (item) => !idsDeviceLocal.includes(item.id)
    ); // Loại các item giống id với local

    devices.value = devicesData.concat(getDeviceLocal()); // Hợp nhất mảng
  } catch (error) {
    console.log(error);
  }
};

const updateDeviceData = async () => {
  try {
    isConfirmationCodeModalOpen.value = false;
    const res = await axios.post(API.UPDATE_DATA, {
      value: devices.value,
      code: confirmationCode.value,
    });
    const data = res.data;

    if (data.status === "success") {
      toast.success(data.message);
    }

    if (data.status === "error") {
      toast.error(data.message);
    }
  } catch (error) {
    console.log(error);
  }
};

const updateCommandFn = (data) => {
  const index = devices.value.findIndex(
    (item) => item.id === deviceDetailID.value
  );
  if (index !== -1) {
    devices.value[index].commands = data;
    localStorage.setItem("deviceData", JSON.stringify(devices.value));
  }
};

const connectBLE = async () => {
  try {
    if (statusBLE.value) return;

    const serviceUuid = "12345678-1234-1234-1234-123456789abc"; // UUID của dịch vụ trên ESP32
    const characteristicUuid = "abcd1234-1234-1234-1234-123456789abc"; // UUID của characteristic trên ESP32

    // Yêu cầu kết nối với thiết bị BLE
    bleDevice = await navigator.bluetooth.requestDevice({
      filters: [{ namePrefix: "MIN" }], // Thay đổi cho phù hợp với tên thiết bị BLE
      optionalServices: [serviceUuid],
    });

    const server = await bleDevice.gatt.connect();
    const service = await server.getPrimaryService(serviceUuid);
    characteristic = await service.getCharacteristic(characteristicUuid);

    statusBLE.value = true;
  } catch (error) {
    console.error("Connection failed: ", error);
    toast.error("Lỗi kết nối");
  }
};

const listenIR = async () => {
  try {
    codeListenIR.value = null;
    const encoder = new TextEncoder();
    const data = encoder.encode("listenIR");
    await characteristic.writeValue(data);
    litenIRStatus.value = "Đang lắng nghe";

    const intervalID = setInterval(async () => {
      try {
        const value = await characteristic.readValue();
        const data = new TextDecoder().decode(value);

        if (data == "listenIR") {
          litenIRStatus.value = "Đang lắng nghe";
        } else {
          codeListenIR.value = data;
          litenIRStatus.value = "Tắt";
          clearInterval(intervalID);
        }
      } catch (error) {
        console.log("Error listen IR: ", error);
      }
    }, 1000);
  } catch (error) {
    console.error("Failed to listen data: ", error);
    toast.error("Lỗi lắng nghe");
  }
};

const sendIR = async (d) => {
  try {
    const encoder = new TextEncoder();
    const data = encoder.encode(d);
    await characteristic.writeValue(data);
    toast.success("Đã gửi IR");
  } catch (error) {
    console.error("Failed to send data: ", error);
    toast.error("Lỗi gửi IR");
  }
};

const runCommanFn = (data) => {
  sendIR(data);
};

getDeviceData();
</script>
