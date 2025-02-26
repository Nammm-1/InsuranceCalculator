<template>
  <div class="min-h-screen bg-gradient-to-b from-gray-50 to-gray-100">
    <div class="container mx-auto px-4 py-12">
      <div class="max-w-4xl mx-auto">
        <div class="text-center mb-12">
          <h1 class="text-4xl font-bold bg-gradient-to-r from-primary to-primary/70 bg-clip-text text-transparent mb-4">
            Bulk Insurance Calculator
          </h1>
          <p class="text-muted-foreground text-lg">
            Calculate premiums for multiple vehicles by uploading a CSV file
          </p>
        </div>

        <div class="grid gap-8">
          <!-- File Upload Section -->
          <div class="p-6 bg-white rounded-lg shadow-lg">
            <div class="space-y-6">
              <div class="flex items-center gap-3 mb-6">
                <upload-icon class="w-8 h-8 text-primary" />
                <h2 class="text-2xl font-semibold">CSV File Upload</h2>
              </div>

              <!-- Instructions -->
              <div class="space-y-4">
                <div class="p-4 bg-primary/5 rounded-lg">
                  <h3 class="font-medium mb-2">CSV File Requirements</h3>
                  <p class="text-sm text-muted-foreground mb-2">
                    Your CSV file should contain the following columns:
                  </p>
                  <ul class="text-sm text-muted-foreground list-disc list-inside">
                    <li>vehicleType (private_individual, private_corporate, taxi)</li>
                    <li>currency (GHC, USD, EUR)</li>
                    <li>exchangeRate (numeric)</li>
                    <li>daysInsured (1-365)</li>
                    <li>propertyDamageCoverage (min 6000)</li>
                    <li>vehicleValue (numeric)</li>
                  </ul>
                </div>

                <!-- Example Format -->
                <div class="p-4 border border-gray-200 rounded-lg">
                  <h3 class="font-medium mb-2">Example CSV Format</h3>
                  <pre class="text-sm bg-gray-50 p-2 rounded overflow-x-auto">
vehicleType,currency,exchangeRate,daysInsured,propertyDamageCoverage,vehicleValue
private_individual,GHC,1,365,6000,50000
taxi,USD,12.5,180,8000,35000</pre>
                </div>

                <!-- Upload Area -->
                <div class="border-2 border-dashed border-gray-300 rounded-lg p-8 text-center">
                  <input type="file" ref="fileInput" accept=".csv" class="hidden" @change="handleFileUpload" />
                  <div class="space-y-4">
                    <upload-cloud-icon class="w-12 h-12 text-gray-400 mx-auto" />
                    <div class="space-y-2">
                      <h3 class="text-xl font-medium">Upload CSV File</h3>
                      <p class="text-sm text-muted-foreground">
                        Click to select or drag and drop your CSV file here
                      </p>
                    </div>
                    <button class="px-4 py-2 bg-primary text-white rounded-md hover:bg-primary/90"
                      @click="() => fileInput?.click()">
                      Select File
                    </button>
                  </div>
                </div>

                <!-- Error Message -->
                <div v-if="error" class="p-4 bg-red-50 text-red-600 rounded-lg">
                  {{ error }}
                </div>

                <!-- Success and Process Button -->
                <div v-if="fileUploaded" class="space-y-4">
                  <div class="flex items-center gap-2 text-sm text-green-600">
                    <check-circle-icon class="w-5 h-5" />
                    <span>File uploaded successfully</span>
                  </div>
                  <button class="w-full px-4 py-2 bg-primary text-white rounded-md hover:bg-primary/90"
                    @click="processFile" :disabled="processing">
                    {{ processing ? 'Processing...' : 'Calculate Premiums' }}
                  </button>
                </div>
              </div>
            </div>
          </div>

          <!-- Results Section -->
          <div v-if="results.length > 0" class="space-y-6">
            <div class="p-6 bg-white rounded-lg shadow-lg">
              <h2 class="text-2xl font-semibold mb-6">Calculation Results</h2>
              <div class="overflow-x-auto">
                <table class="w-full">
                  <thead>
                    <tr class="border-b">
                      <th class="py-2 text-left">Vehicle Type</th>
                      <th class="py-2 text-right">Premium</th>
                      <th class="py-2 text-right">Third Party Rate</th>
                      <th class="py-2 text-right">Currency</th>
                    </tr>
                  </thead>
                  <tbody>
                    <tr v-for="(result, index) in results" :key="index" class="border-b">
                      <td class="py-2">{{ formatVehicleType(result.vehicleType) }}</td>
                      <td class="py-2 text-right">{{ formatNumber(result.premium) }}</td>
                      <td class="py-2 text-right">{{ formatNumber(result.thirdPartyRate) }}</td>
                      <td class="py-2 text-right">{{ result.currency }}</td>
                    </tr>
                  </tbody>
                  <tfoot>
                    <tr class="font-bold">
                      <td class="py-2">Total Premium</td>
                      <td class="py-2 text-right">{{ formatNumber(totalPremium) }}</td>
                      <td colspan="2"></td>
                    </tr>
                  </tfoot>
                </table>
              </div>
            </div>

            <!-- Download Results Button -->
            <button class="w-full px-4 py-2 bg-primary text-white rounded-md hover:bg-primary/90"
              @click="downloadResults">
              Download Results
            </button>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed } from 'vue'
import { Upload, UploadCloud, CheckCircle } from 'lucide-vue-next'

const fileInput = ref(null)
const fileUploaded = ref(false)
const processing = ref(false)
const results = ref([])
const error = ref(null)

const totalPremium = computed(() => {
  return results.value.reduce((sum, result) => sum + result.premium, 0)
})

const handleFileUpload = (event) => {
  const input = event.target
  if (input.files?.length) {
    const file = input.files[0]
    if (file.type !== 'text/csv' && !file.name.endsWith('.csv')) {
      error.value = 'Please upload a CSV file'
      return
    }
    fileUploaded.value = true
    error.value = null
  }
}

const formatNumber = (num) => {
  return num.toLocaleString(undefined, {
    minimumFractionDigits: 2,
    maximumFractionDigits: 2,
  })
}

const formatVehicleType = (type) => {
  return type.split('_').map(word =>
    word.charAt(0).toUpperCase() + word.slice(1)
  ).join(' ')
}

const processFile = async () => {
  if (!fileInput.value?.files?.length) return

  processing.value = true
  error.value = null

  const formData = new FormData()
  formData.append('file', fileInput.value.files[0])

  try {
    const response = await fetch('/api/calculate/bulk', {
      method: 'POST',
      body: formData
    })

    if (!response.ok) {
      throw new Error('Failed to process file')
    }

    const data = await response.json()
    results.value = data.results
  } catch (err) {
    error.value = 'Error processing file. Please check the format and try again.'
  } finally {
    processing.value = false
  }
}

const downloadResults = () => {
  const headers = ['Vehicle Type', 'Premium', 'Third Party Rate', 'Currency']
  const rows = results.value.map(result => [
    formatVehicleType(result.vehicleType),
    result.premium,
    result.thirdPartyRate,
    result.currency
  ])

  const csv = [
    headers.join(','),
    ...rows.map(row => row.join(','))
  ].join('\n')

  const blob = new Blob([csv], { type: 'text/csv' })
  const url = window.URL.createObjectURL(blob)
  const a = document.createElement('a')
  a.href = url
  a.download = 'insurance-calculations.csv'
  a.click()
  window.URL.revokeObjectURL(url)
}
</script>
