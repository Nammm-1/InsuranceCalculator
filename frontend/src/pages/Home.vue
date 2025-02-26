<template>
  <div class="min-h-screen bg-gradient-to-b from-gray-50 to-gray-100">
    <div class="container mx-auto px-4 py-12">
      <div class="max-w-4xl mx-auto">
        <div class="text-center mb-12">
          <h1 class="text-4xl font-bold bg-gradient-to-r from-primary to-primary/70 bg-clip-text text-transparent mb-4">
            Vehicle Insurance Calculator
          </h1>
          <p class="text-muted-foreground text-lg">
            Calculate your vehicle insurance premium instantly
          </p>
        </div>

        <div class="grid md:grid-cols-2 gap-8 items-start">
          <Card class="p-6 shadow-lg">
            <div class="flex items-center gap-3 mb-6">
              <Car class="w-8 h-8 text-primary" />
              <h2 class="text-2xl font-semibold">Features</h2>
            </div>
            <ul class="space-y-3 text-muted-foreground">
              <li>✓ Multiple vehicle types support</li>
              <li>✓ Multi-currency calculations (GHC, USD, EUR)</li>
              <li>✓ Flexible insurance duration (1-365 days)</li>
              <li>✓ Comprehensive property damage coverage</li>
              <li>✓ Instant premium calculation</li>
            </ul>
          <!-- </Card>

          <Card class="p-6 shadow-lg"> -->
            <InsuranceForm />
          </Card>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { Card } from 'src/components/ui/card.vue'
import InsuranceForm from 'src/components/insuranceForm.vue'
import { Car } from 'lucide-vue-next'
</script>
