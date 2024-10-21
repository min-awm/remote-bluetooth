<template>
    <div class="container max-w-4xl p-4 mx-auto">
      <div class="flex items-center justify-between mb-6">
        <router-link
          to="/"
          class="px-4 py-2 text-gray-700 transition duration-300 ease-in-out bg-gray-200 rounded-md hover:bg-gray-300 focus:outline-none focus:ring-2 focus:ring-gray-500 focus:ring-offset-2"
        >
          Quay lại
        </router-link>
        <p><strong>Bluetooth:</strong> {{ device.isOn ? 'Đang bật' : 'Đang tắt' }}</p>
        
      </div>
      
      <div class="p-4 mb-6 bg-white rounded-lg shadow-md">
        <h2 class="mb-4 text-xl font-semibold">Thông tin thiết bị</h2>
        <p><strong>Loại:</strong> {{ device.type }}</p>
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
              <strong>Mã lệnh:</strong> {{ command.code }}
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
      <div v-if="isModalOpen" class="fixed inset-0 flex items-center justify-center p-4 bg-black bg-opacity-50">
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
  import { ref, computed } from 'vue'
  import { useRoute } from 'vue-router'
  import { PlusCircle } from 'lucide-vue-next'
  
  const route = useRoute()
  const devices = ref([]) // Giả sử đây là store chứa tất cả các thiết bị
  
  const device = computed(() => {
    return devices.value.find(d => d.id === parseInt(route.params.id)) || { name: '', type: '', isOn: false, commands: [] }
  })
  
  const isModalOpen = ref(false)
  const newCommand = ref({ name: '', code: '' })
  
  const openModal = () => {
    isModalOpen.value = true
  }
  
  const closeModal = () => {
    isModalOpen.value = false
    newCommand.value = { name: '', code: '' }
  }
  
  const addCommand = () => {
    if (newCommand.value.name.trim() && newCommand.value.code.trim()) {
      device.value.commands.push({
        id: Date.now(),
        name: newCommand.value.name.trim(),
        code: newCommand.value.code.trim()
      })
      closeModal()
    }
  }
  
  const executeCommand = (commandId) => {
    const command = device.value.commands.find(c => c.id === commandId)
    if (command) {
      console.log(`Executing command: ${command.name} (Code: ${command.code}) for device ${device.value.name}`)
      // Implement the logic to execute the command here
    }
  }
  </script>