<template>
  <q-page padding>
    <div class="text-center q-mb-xl">
      <div class="text-h3 text-primary q-mb-md">Vehicle Insurance Calculator</div>
      <div class="text-h6 text-grey-7">Calculate your vehicle insurance premium instantly</div>
    </div>

    <div class="row q-col-gutter-xl">
      <!-- Features Section -->
      <div class="col-12 col-md-5">
        <q-card flat bordered class="features-card">
          <q-card-section>
            <div class="row items-center q-mb-lg">
              <q-icon name="directions_car" size="2rem" color="primary" class="q-mr-md" />
              <div class="text-h5">Features</div>
            </div>

            <div class="features-list q-gutter-y-md">
              <div class="feature-item">
                <q-icon name="check" color="primary" />
                <span>Multiple vehicle types support</span>
              </div>
              <div class="feature-item">
                <q-icon name="check" color="primary" />
                <span>Multi-currency calculations (GHC, USD, EUR)</span>
              </div>
              <div class="feature-item">
                <q-icon name="check" color="primary" />
                <span>Flexible insurance duration (1-365 days)</span>
              </div>
              <div class="feature-item">
                <q-icon name="check" color="primary" />
                <span>Comprehensive property damage coverage</span>
              </div>
              <div class="feature-item">
                <q-icon name="check" color="primary" />
                <span>Instant premium calculation</span>
              </div>
            </div>
          </q-card-section>
        </q-card>
      </div>

      <!-- Calculator Form -->
      <div class="col-12 col-md-7">
        <q-card flat bordered>
          <q-card-section>
            <q-form @submit.prevent="onSubmit" class="q-gutter-md">
              <!-- Vehicle Type Selection -->
              <div class="input-group">
                <div class="text-subtitle1 q-mb-sm">Vehicle Type</div>
                <q-select
                  v-model="selectedVehicleType"
                  :options="VEHICLE_TYPES"
                  outlined
                  dense
                  option-label="label"
                  @update:model-value="vehicleType = $event"
                  :rules="[val => !!val || 'Vehicle type is required']"
                />
              </div>

              <!-- Currency Selection -->
              <div class="input-group">
                <div class="text-subtitle1 q-mb-sm">Currency</div>
                <div class="row q-col-gutter-sm">
                  <div class="col-12 col-sm-6">
                    <q-select
                      v-model="selectedCurrency"
                      :options="CURRENCIES"
                      outlined
                      dense
                      option-label="label"
                      @update:model-value="currency = $event"
                      :rules="[val => !!val || 'Currency is required']"
                    />
                  </div>
                  <div class="col-12 col-sm-6">
                    <q-input
                      v-if="selectedCurrency?.value !== 'GHC'"
                      v-model.number="exchangeRate"
                      type="number"
                      outlined
                      dense
                      label="Exchange Rate"
                      :rules="[
                        val => !!val || 'Exchange rate is required',
                        val => val > 0 || 'Exchange rate must be positive'
                      ]"
                    />
                  </div>
                </div>
              </div>

              <!-- Days to Insure -->
              <div class="input-group">
                <div class="text-subtitle1 q-mb-sm">Days to Insure (1-365)</div>
                <q-input
                  v-model.number="daysInsured"
                  type="number"
                  outlined
                  dense
                  :rules="[
                    val => !!val || 'Insurance period is required',
                    val => val > 0 && val <= 365 || 'Days must be between 1 and 365'
                  ]"
                />
              </div>

              <!-- Property Damage Coverage -->
              <div class="input-group">
                <div class="text-subtitle1 q-mb-sm">Property Damage Coverage (min 6000 GHC)</div>
                <q-input
                  v-model.number="propertyDamageCoverage"
                  type="number"
                  outlined
                  dense
                  :rules="[
                    val => !!val || 'Coverage amount is required',
                    val => val >= 6000 || 'Minimum coverage is 6000 GHC'
                  ]"
                />
              </div>

              <!-- Vehicle Value -->
              <div class="input-group">
                <div class="text-subtitle1 q-mb-sm">Vehicle Value</div>
                <q-input
                  v-model.number="vehicleValue"
                  type="number"
                  outlined
                  dense
                  :rules="[
                    val => !!val || 'Vehicle value is required',
                    val => val > 0 || 'Vehicle value must be positive'
                  ]"
                />
              </div>

              <div class="row justify-center q-mt-lg">
                <q-btn
                  label="Calculate Premium"
                  type="submit"
                  color="primary"
                  size="lg"
                  :loading="loading"
                  class="full-width"
                />
              </div>
            </q-form>
          </q-card-section>
        </q-card>

        <!-- Results Card -->
        <q-card v-if="calculations.length" flat bordered class="q-mt-md">
          <q-card-section>
            <div class="text-h6 q-mb-md">Results</div>
            <div v-for="(calc, index) in calculations" :key="index">
              <q-separator v-if="index > 0" class="q-my-md" />

              <div class="row justify-between q-mb-sm">
                <div>Third-Party Rate:</div>
                <div>{{ formatCurrency(calc.thirdPartyRate, calc.currency) }}</div>
              </div>

              <div class="row justify-between q-mb-sm">
                <div>Insurance Premium:</div>
                <div>{{ formatCurrency(calc.premium, calc.currency) }}</div>
              </div>

              <div class="row justify-between text-bold">
                <div>Total Cost:</div>
                <div>{{ formatCurrency(calc.totalCost, calc.currency) }}</div>
              </div>
            </div>

            <q-separator class="q-my-md" />

            <div class="row justify-between text-h6">
              <div>Grand Total:</div>
              <div>{{ formatCurrency(grandTotal, calculations[0]?.currency) }}</div>
            </div>
          </q-card-section>

          <q-card-actions align="right">
            <q-btn
              label="Clear All"
              color="negative"
              flat
              @click="resetAll"
            />
          </q-card-actions>
        </q-card>
      </div>
    </div>
  </q-page>
</template>

<script>
import { defineComponent, ref } from 'vue';
import { useCalculator } from '../composables/useCalculator';
import { VEHICLE_TYPES, CURRENCIES } from '../constants';

export default defineComponent({
  name: 'CalculatorPage',

  setup() {
    const {
      vehicleType,
      currency,
      exchangeRate,
      daysInsured,
      propertyDamageCoverage,
      vehicleValue,
      calculations,
      grandTotal,
      calculatePremium,
      reset,
      resetAll
    } = useCalculator();

    const loading = ref(false);
    const selectedVehicleType = ref(null);
    const selectedCurrency = ref(null);

    const formatCurrency = (value, currencyCode) => {
      return new Intl.NumberFormat('en-US', {
        style: 'currency',
        currency: currencyCode
      }).format(value);
    };

    const onSubmit = async () => {
      loading.value = true;
      try {
        calculatePremium();
        selectedVehicleType.value = null;
        selectedCurrency.value = null;
      } finally {
        loading.value = false;
      }
    };

    return {
      VEHICLE_TYPES,
      CURRENCIES,
      selectedVehicleType,
      selectedCurrency,
      vehicleType,
      currency,
      exchangeRate,
      daysInsured,
      propertyDamageCoverage,
      vehicleValue,
      calculations,
      grandTotal,
      loading,
      onSubmit,
      reset,
      resetAll,
      formatCurrency
    };
  }
});
</script>

<style scoped>
.features-card {
  height: 100%;
}

.feature-item {
  display: flex;
  align-items: center;
  gap: 0.5rem;
  font-size: 1.1rem;
  color: #666;
}

.input-group {
  margin-bottom: 1.5rem;
}
</style>
