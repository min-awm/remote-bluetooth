<template>
  <div class="container max-w-4xl p-4 mx-auto">
    <h1 class="mb-6 text-2xl font-bold text-center md:text-3xl">
      Ứng dụng điều khiển đa năng
    </h1>

    <div class="p-4 mb-6 bg-white rounded-lg shadow-md">
      <div class="flex gap-4 pb-5">
        <button
          class="w-auto px-4 py-2 text-white transition duration-300 ease-in-out bg-blue-500 rounded-md hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-offset-2"
        >
          <Bluetooth class="inline-block w-4 h-4 mr-1" />
          Kết nối bluetooth
        </button>
      </div>

      <h2 class="mb-4 text-xl font-semibold">Gửi mã IR</h2>
      <div
        class="flex flex-col space-y-2 md:flex-row md:space-y-0 md:space-x-2"
      >
        <input
          type="text"
          placeholder="Nhập mã"
          class="flex-grow px-3 py-2 border rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500"
        />

        <button
          @click="addDevice"
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
  </div>

  <div class="container max-w-4xl p-4 mx-auto">
    <div class="flex items-center justify-between mb-6">
      <router-link
        to="/"
        class="px-4 py-2 text-gray-700 transition duration-300 ease-in-out bg-gray-200 rounded-md hover:bg-gray-300 focus:outline-none focus:ring-2 focus:ring-gray-500 focus:ring-offset-2"
      >
        Quay lại
      </router-link>
      <p>
        <strong>Bluetooth:</strong>
        {{ device.isOn ? "Đang bật" : "Đang tắt" }}
      </p>
    </div>

    <div class="p-4 mb-6 bg-white rounded-lg shadow-md">
      <h2 class="mb-4 text-xl font-semibold">Thông tin thiết bị</h2>
      <p>
        <strong>Loại:</strong>
        {{ device.type }}
      </p>
    </div>

    <div class="p-4 mb-6 bg-white rounded-lg shadow-md">
      <h2 class="mb-4 text-xl font-semibold">Danh sách lệnh</h2>
      <ul v-if="device.commands.length > 0" class="space-y-4">
        <li
          v-for="command in device.commands"
          :key="command.id"
          class="p-4 bg-gray-100 rounded-md"
        >
          <div class="flex items-center justify-between mb-2">
            <span class="font-semibold">{{ command.name }}</span>
            <button
              @click="executeCommand(command.id)"
              class="px-3 py-1 text-white transition duration-300 ease-in-out bg-blue-500 rounded-md hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-offset-2"
            >
              Thực hiện
            </button>
          </div>
          <div class="text-sm text-gray-600">
            <strong>Mã lệnh:</strong>
            {{ command.code }}
          </div>
        </li>
      </ul>
      <p v-else class="text-gray-500">Chưa có lệnh nào được thêm vào.</p>
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
        <h2 class="mb-4 text-xl font-semibold">Thêm lệnh mới</h2>
        <input
          v-model="newCommand.name"
          type="text"
          placeholder="Tên lệnh"
          class="w-full px-3 py-2 mb-2 border rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500"
        />
        <input
          v-model="newCommand.code"
          type="text"
          placeholder="Mã lệnh"
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
            @click="addCommand"
            class="px-4 py-2 text-white transition duration-300 ease-in-out bg-blue-500 rounded-md hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-offset-2"
          >
            Thêm
          </button>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, reactive } from "vue";
import {
  PlusCircle,
  Trash,
  Send,
  Bluetooth,
  CloudUpload,
} from "lucide-vue-next";
import API from "@/api";
import axios from "@/axios";

const devices = ref([]);
const newDevice = reactive({
  name: "",
  type: "",
});
const isModalOpen = ref(false);

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
      command: [],
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

    devices.value = data.concat(getDeviceLocal());
  } catch (error) {
    console.log(error);
  }
};

getDeviceData();
</script>
