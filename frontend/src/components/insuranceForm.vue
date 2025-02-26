<template>
  <div>
    <form v-if="showForm" @submit.prevent="onSubmit" class="space-y-6">
      <div class="space-y-4">
        <div>
          <label class="text-sm font-medium">Vehicle Type</label>
          <Select v-model="form.vehicleType">
            <SelectTrigger class="mt-1">
              <SelectValue />
            </SelectTrigger>
            <SelectContent>
              <SelectItem value="private_individual">Private Individual</SelectItem>
              <SelectItem value="private_corporate">Private Corporate</SelectItem>
              <SelectItem value="taxi">Taxi</SelectItem>
            </SelectContent>
          </Select>
        </div>

        <div>
          <label class="text-sm font-medium">Currency</label>
          <Select v-model="form.currency" @update:modelValue="handleCurrencyChange">
            <SelectTrigger class="mt-1">
              <SelectValue />
            </SelectTrigger>
            <SelectContent>
              <SelectItem value="GHC">GHC</SelectItem>
              <SelectItem value="USD">USD</SelectItem>
              <SelectItem value="EUR">EUR</SelectItem>
            </SelectContent>
          </Select>
        </div>

        <div>
          <label class="text-sm font-medium">Days to Insure (1-365)</label>
          <Input v-model.number="form.daysInsured" type="number" class="mt-1" :min="1" :max="365" />
        </div>

        <div>
          <label class="text-sm font-medium">Property Damage Coverage (min 6000 GHC)</label>
          <Input v-model.number="form.propertyDamageCoverage" type="number" class="mt-1" :min="6000" />
        </div>

        <div>
          <label class="text-sm font-medium">Vehicle Value</label>
          <Input v-model.number="form.vehicleValue" type="number" class="mt-1" :min="1" />
        </div>
      </div>

      <Button type="submit" class="w-full" :disabled="calculating">
        {{ calculating ? 'Calculating...' : 'Calculate Premium' }}
      </Button>
    </form>

    <div v-else class="space-y-6">
      <CalculationResult v-for="(result, idx) in results" :key="result.id" :result="result" :index="idx + 1"
        :is-last="idx === results.length - 1" />

      <div class="flex gap-4">
        <Button @click="handleCalculateAnother" class="flex-1" variant="outline">
          Calculate Another Premium
        </Button>
        <Button @click="handleFinish" class="flex-1">
          Finish
        </Button>
      </div>

      <div v-if="results.length > 1" class="mt-6 p-4 bg-primary/10 rounded-lg">
        <h3 class="text-xl font-bold text-primary">Running Total</h3>
        <p class="text-2xl font-bold mt-2">
          {{ results[0].currency }} {{ formatCurrency(totalPremium) }}
        </p>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed } from 'vue'
import { useToast } from '@/composables/use-toast'
import type { Calculator } from '@shared/schema'
import type { PremiumResult } from '@/types'
import { Button, Input } from '@radix-vue'
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from '@/components/ui/select.vue'
import CalculationResult from './calculation-result.vue'

const toast = useToast()
const results = ref<PremiumResult[]>([])
const exchangeRate = ref(1)
const showForm = ref(true)
const calculating = ref(false)

const form = ref<Calculator>({
  vehicleType: 'private_individual',
  currency: 'GHC',
  exchangeRate: 1,
  daysInsured: 365,
  propertyDamageCoverage: 6000,
  vehicleValue: 0
})

const totalPremium = computed(() =>
  results.value.reduce((sum, result) => sum + result.premium, 0)
)

const formatCurrency = (amount: number) => {
  return amount.toLocaleString(undefined, {
    minimumFractionDigits: 2,
    maximumFractionDigits: 2,
  })
}

const handleCurrencyChange = async (value: string) => {
  if (value !== 'GHC') {
    const rate = window.prompt('Enter exchange rate:')
    if (rate) {
      const rateNum = parseFloat(rate)
      if (!isNaN(rateNum) && rateNum > 0) {
        exchangeRate.value = rateNum
        form.value.exchangeRate = rateNum
      }
    }
  } else {
    exchangeRate.value = 1
    form.value.exchangeRate = 1
  }
}

const onSubmit = async () => {
  calculating.value = true
  try {
    const response = await fetch('/api/calculate', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(form.value)
    })
    const data = await response.json()
    results.value.push(data)
    showForm.value = false
    toast.success('Calculation Complete', 'Your insurance premium has been calculated.')
  } catch (error) {
    toast.error('Error', 'Failed to calculate premium. Please try again.')
  } finally {
    calculating.value = false
  }
}

const handleCalculateAnother = () => {
  form.value = {
    vehicleType: 'private_individual',
    currency: 'GHC',
    exchangeRate: 1,
    daysInsured: 365,
    propertyDamageCoverage: 6000,
    vehicleValue: 0
  }
  showForm.value = true
}

const handleFinish = () => {
  showForm.value = false
}
</script>
