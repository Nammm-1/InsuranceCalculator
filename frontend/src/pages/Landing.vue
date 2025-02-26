<template>
  <div class="min-h-screen bg-gradient-to-b from-gray-50 to-gray-100">
    <div class="container mx-auto px-4 py-12">
      <div class="max-w-4xl mx-auto">
        <div class="text-center mb-12">
          <h1 class="text-4xl font-bold bg-gradient-to-r from-primary to-primary/70 bg-clip-text text-transparent mb-4">
            Vehicle Insurance Calculator
          </h1>
          <p class="text-muted-foreground text-lg">
            Choose your calculation method
          </p>
        </div>

        <div class="grid md:grid-cols-2 gap-8">
          <router-link to="/ind" class="block">
            <div class="p-6 bg-white rounded-lg shadow-lg hover:shadow-xl transition-shadow">
              <div class="flex items-center gap-3 mb-6">
                <calculator-icon class="w-8 h-8 text-primary" />
                <h2 class="text-2xl font-semibold">Normal Calculator</h2>
              </div>
              <p class="text-muted-foreground mb-4">
                Calculate insurance premium for individual vehicles with detailed options and immediate results.
              </p>
              <ul class="space-y-2 text-sm text-muted-foreground">
                <li>✓ Single vehicle calculation</li>
                <li>✓ Interactive form</li>
                <li>✓ Instant results</li>
              </ul>
            </div>
          </router-link>

          <router-link to="/bulk-calculator" class="block">
            <div class="p-6 bg-white rounded-lg shadow-lg hover:shadow-xl transition-shadow">
              <div class="flex items-center gap-3 mb-6">
                <files-icon class="w-8 h-8 text-primary" />
                <h2 class="text-2xl font-semibold">Bulk Calculator</h2>
              </div>
              <p class="text-muted-foreground mb-4">
                Process multiple vehicle calculations at once using a file upload system.
              </p>
              <ul class="space-y-2 text-sm text-muted-foreground">
                <li>✓ Multiple vehicle processing</li>
                <li>✓ File upload support</li>
                <li>✓ Batch results</li>
              </ul>
            </div>
          </router-link>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { Calculator as CalculatorIcon, Files as FilesIcon } from 'lucide-vue-next'
</script>
