<template>
  <div class="container max-w-4xl p-4 mx-auto">
    <h1 class="mb-6 text-2xl font-bold text-center md:text-3xl">Ứng dụng Điều khiển Đa năng</h1>
    
    <div class="p-4 mb-6 bg-white rounded-lg shadow-md">
      <h2 class="mb-4 text-xl font-semibold">Thêm thiết bị mới</h2>
      <div class="flex flex-col space-y-2 md:flex-row md:space-y-0 md:space-x-2">
        <input
          v-model="newDevice.name"
          type="text"
          placeholder="Tên thiết bị"
          class="flex-grow px-3 py-2 border rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500"
        />
        <input
          v-model="newDevice.type"
          type="text"
          placeholder="Loại thiết bị"
          class="flex-grow px-3 py-2 border rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500"
        />
        <button
          @click="addDevice"
          class="w-full px-4 py-2 text-white transition duration-300 ease-in-out bg-blue-500 rounded-md md:w-auto hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-offset-2"
        >
          <PlusCircle class="inline-block w-4 h-4 mr-2" />
          Thêm
        </button>
      </div>
    </div>

    <div class="grid grid-cols-1 gap-4 sm:grid-cols-2 lg:grid-cols-3">
      <div
        v-for="device in devices"
        :key="device.id"
        class="p-4 transition duration-300 ease-in-out bg-white rounded-lg shadow-md hover:shadow-lg"
      >
        <h3 class="mb-2 text-lg font-semibold">{{ device.name }}</h3>
        <p class="mb-4 text-sm text-gray-500">Loại: {{ device.type }}</p>
        <button
          @click="toggleDevice(device.id)"
          :class="[
            'w-full px-4 py-2 rounded-md focus:outline-none focus:ring-2 focus:ring-offset-2 transition duration-300 ease-in-out',
            device.isOn
              ? 'bg-green-500 text-white hover:bg-green-600 focus:ring-green-500'
              : 'bg-gray-200 text-gray-700 hover:bg-gray-300 focus:ring-gray-500'
          ]"
        >
          <Power class="inline-block w-4 h-4 mr-2" />
          {{ device.isOn ? 'Tắt' : 'Bật' }}
        </button>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, reactive } from 'vue'
import { PlusCircle, Power } from 'lucide-vue-next'

const devices = ref([])
const newDevice = reactive({
  name: '',
  type: ''
})

const addDevice = () => {
  if (newDevice.name && newDevice.type) {
    devices.value.push({
      id: Date.now(),
      name: newDevice.name,
      type: newDevice.type,
      isOn: false
    })
    newDevice.name = ''
    newDevice.type = ''
  }
}

const toggleDevice = (id) => {
  const device = devices.value.find(d => d.id === id)
  if (device) {
    device.isOn = !device.isOn
  }
}
</script>